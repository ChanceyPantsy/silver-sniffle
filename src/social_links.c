#include "global.h"
#include "battle.h"
#include "event_data.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "script.h"
#include "social_links.h"
#include "config/social_links.h"
#include "constants/battle.h"

#include "data/social_links.h"

// Total points needed to reach each rank.
static const u16 sRankPointThresholds[SOCIAL_LINK_MAX_RANK + 1] =
{
    [0]  = 0,
    [1]  = 10,
    [2]  = 25,
    [3]  = 45,
    [4]  = 70,
    [5]  = 100,
    [6]  = 135,
    [7]  = 175,
    [8]  = 220,
    [9]  = 270,
    [10] = 330,
};

struct PreBattleLevel
{
    u32 personality;
    u8 level;
};

static EWRAM_DATA struct PreBattleLevel sPreBattleLevels[PARTY_SIZE] = {0};

static inline bool32 IsValidLinkId(u32 linkId)
{
    return linkId < NUM_SOCIAL_LINKS;
}

static inline bool32 IsPokemonLink(u32 linkId)
{
    return linkId < NUM_POKEMON_SOCIAL_LINKS;
}

void ResetSocialLinks(void)
{
    memset(&gSaveBlock3Ptr->socialLinks, 0, sizeof(gSaveBlock3Ptr->socialLinks));
}

const u8 *SocialLink_GetName(u32 linkId)
{
    if (!IsValidLinkId(linkId))
        return COMPOUND_STRING("???");
    return gSocialLinksInfo[linkId].name;
}

enum Type SocialLink_GetType(u32 linkId)
{
    if (!IsValidLinkId(linkId))
        return TYPE_NONE;
    if (IsPokemonLink(linkId))
    {
        const struct SocialLinkMonBinding *binding = &gSaveBlock3Ptr->socialLinks.mons[linkId];
        return binding->isBound ? binding->type : TYPE_NONE;
    }
    return gSocialLinksInfo[linkId].type;
}

u32 SocialLink_GetPoints(u32 linkId)
{
    if (!IsValidLinkId(linkId))
        return 0;
    return gSaveBlock3Ptr->socialLinks.points[linkId];
}

u32 SocialLink_GetPointsForRank(u32 rank)
{
    return sRankPointThresholds[min(rank, SOCIAL_LINK_MAX_RANK)];
}

u32 SocialLink_GetRank(u32 linkId)
{
    u32 points = SocialLink_GetPoints(linkId);
    u32 rank = 0;

    while (rank < SOCIAL_LINK_MAX_RANK && points >= sRankPointThresholds[rank + 1])
        rank++;
    return rank;
}

u32 SocialLink_GetRankCap(u32 linkId)
{
    if (!IsValidLinkId(linkId))
        return 0;

    const struct SocialLinkInfo *info = &gSocialLinksInfo[linkId];
    u32 cap = info->baseRankCap;

    for (const struct SocialLinkRankGate *gate = info->gates; gate != NULL && gate->gateFlag != 0; gate++)
    {
        if (gate->rankCap > cap && FlagGet(gate->gateFlag))
            cap = gate->rankCap;
    }
    return min(cap, SOCIAL_LINK_MAX_RANK);
}

// Returns TRUE if the link went up at least one rank.
bool32 SocialLink_AddPoints(u32 linkId, u32 points)
{
    if (!SOCIAL_LINKS_ENABLED || !IsValidLinkId(linkId) || points == 0)
        return FALSE;
    if (IsPokemonLink(linkId) && !SocialLink_IsMonBound(linkId))
        return FALSE;

    u32 oldRank = SocialLink_GetRank(linkId);
    u32 current = gSaveBlock3Ptr->socialLinks.points[linkId];
    u32 maxPoints = sRankPointThresholds[SocialLink_GetRankCap(linkId)];

    // Points over the cap are discarded. Never lower points that were set above the cap by a script.
    if (current < maxPoints)
        gSaveBlock3Ptr->socialLinks.points[linkId] = min(current + points, maxPoints);

    return SocialLink_GetRank(linkId) > oldRank;
}

// Sets a link to exactly the start of a rank, ignoring its cap. Intended for scripted story moments and debugging.
void SocialLink_SetRank(u32 linkId, u32 rank)
{
    if (!IsValidLinkId(linkId))
        return;
    gSaveBlock3Ptr->socialLinks.points[linkId] = SocialLink_GetPointsForRank(rank);
}

bool32 SocialLink_IsMonBound(u32 linkId)
{
    return IsPokemonLink(linkId) && gSaveBlock3Ptr->socialLinks.mons[linkId].isBound;
}

s32 SocialLink_GetLinkForBoxMon(struct BoxPokemon *boxMon)
{
    if (GetBoxMonData(boxMon, MON_DATA_SPECIES) == SPECIES_NONE || GetBoxMonData(boxMon, MON_DATA_IS_EGG))
        return -1;

    u32 personality = GetBoxMonData(boxMon, MON_DATA_PERSONALITY);
    u32 otId = GetBoxMonData(boxMon, MON_DATA_OT_ID);

    for (u32 linkId = 0; linkId < NUM_POKEMON_SOCIAL_LINKS; linkId++)
    {
        const struct SocialLinkMonBinding *binding = &gSaveBlock3Ptr->socialLinks.mons[linkId];
        if (binding->isBound && binding->personality == personality && binding->otId == otId)
            return linkId;
    }
    return -1;
}

// Fails if the link is not a Pokémon link, is already bound, or the Pokémon is already bound to another link.
bool32 SocialLink_BindBoxMon(u32 linkId, struct BoxPokemon *boxMon)
{
    if (!IsPokemonLink(linkId) || SocialLink_IsMonBound(linkId))
        return FALSE;

    enum Species species = GetBoxMonData(boxMon, MON_DATA_SPECIES);
    if (species == SPECIES_NONE || GetBoxMonData(boxMon, MON_DATA_IS_EGG) || SocialLink_GetLinkForBoxMon(boxMon) >= 0)
        return FALSE;

    struct SocialLinkMonBinding *binding = &gSaveBlock3Ptr->socialLinks.mons[linkId];
    binding->personality = GetBoxMonData(boxMon, MON_DATA_PERSONALITY);
    binding->otId = GetBoxMonData(boxMon, MON_DATA_OT_ID);
    binding->type = GetSpeciesType(species, 0);
    binding->isBound = TRUE;
    return TRUE;
}

// Extra shiny rolls from every maxed link that shares a type with the species.
u32 SocialLink_GetShinyRollsForSpecies(enum Species species)
{
    if (!SOCIAL_LINKS_ENABLED || species == SPECIES_NONE)
        return 0;

    enum Type type1 = GetSpeciesType(species, 0);
    enum Type type2 = GetSpeciesType(species, 1);
    u32 rolls = 0;

    for (u32 linkId = 0; linkId < NUM_SOCIAL_LINKS; linkId++)
    {
        enum Type linkType = SocialLink_GetType(linkId);
        if (linkType == TYPE_NONE || (linkType != type1 && linkType != type2))
            continue;
        if (SocialLink_GetRank(linkId) == SOCIAL_LINK_MAX_RANK)
            rolls += SOCIAL_LINK_MAXED_SHINY_ROLLS;
    }
    return rolls;
}

void SocialLink_OnBattleStart(void)
{
    for (u32 slot = 0; slot < PARTY_SIZE; slot++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][slot];
        sPreBattleLevels[slot].personality = GetMonData(mon, MON_DATA_PERSONALITY);
        sPreBattleLevels[slot].level = GetMonData(mon, MON_DATA_LEVEL);
    }
}

static u32 GetLevelsGainedThisBattle(struct Pokemon *mon)
{
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY);
    u32 level = GetMonData(mon, MON_DATA_LEVEL);

    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        if (sPreBattleLevels[i].personality == personality && level > sPreBattleLevels[i].level)
            return level - sPreBattleLevels[i].level;
    }
    return 0;
}

void SocialLink_OnBattleEnd(void)
{
    if (!SOCIAL_LINKS_ENABLED)
        return;

    bool32 won = (gBattleOutcome == B_OUTCOME_WON || gBattleOutcome == B_OUTCOME_CAUGHT);

    for (u32 slot = 0; slot < PARTY_SIZE; slot++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][slot];
        s32 linkId = SocialLink_GetLinkForBoxMon(&mon->box);
        if (linkId < 0)
            continue;

        const struct PartyState *partyState = &gBattleStruct->partyState[B_TRAINER_PLAYER][slot];
        u32 points = GetLevelsGainedThisBattle(mon) * SOCIAL_LINK_POINTS_LEVEL_UP;

        if (won && partyState->sentOut)
        {
            points += SOCIAL_LINK_POINTS_BATTLE;
            if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                points += SOCIAL_LINK_POINTS_TRAINER_BONUS;
            if (!partyState->fainted && GetMonData(mon, MON_DATA_HP) != 0)
                points += SOCIAL_LINK_POINTS_NO_FAINT;
        }
        SocialLink_AddPoints(linkId, points);
    }
}

// Script commands

void ScrCmd_sociallink_addpoints(struct ScriptContext *ctx)
{
    u32 linkId = VarGet(ScriptReadHalfword(ctx));
    u32 points = VarGet(ScriptReadHalfword(ctx));

    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE);
    Script_RequestWriteVar(VAR_RESULT);
    gSpecialVar_Result = SocialLink_AddPoints(linkId, points);
}

void ScrCmd_sociallink_getrank(struct ScriptContext *ctx)
{
    u32 linkId = VarGet(ScriptReadHalfword(ctx));

    Script_RequestEffects(SCREFF_V1);
    Script_RequestWriteVar(VAR_RESULT);
    gSpecialVar_Result = SocialLink_GetRank(linkId);
}

void ScrCmd_sociallink_getrankcap(struct ScriptContext *ctx)
{
    u32 linkId = VarGet(ScriptReadHalfword(ctx));

    Script_RequestEffects(SCREFF_V1);
    Script_RequestWriteVar(VAR_RESULT);
    gSpecialVar_Result = SocialLink_GetRankCap(linkId);
}

void ScrCmd_sociallink_setrank(struct ScriptContext *ctx)
{
    u32 linkId = VarGet(ScriptReadHalfword(ctx));
    u32 rank = VarGet(ScriptReadHalfword(ctx));

    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE);
    SocialLink_SetRank(linkId, rank);
}

void ScrCmd_sociallink_bindpartymon(struct ScriptContext *ctx)
{
    u32 linkId = VarGet(ScriptReadHalfword(ctx));
    u32 slot = VarGet(ScriptReadHalfword(ctx));

    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE);
    Script_RequestWriteVar(VAR_RESULT);
    if (slot < PARTY_SIZE && SocialLink_BindBoxMon(linkId, &gParties[B_TRAINER_PLAYER][slot].box))
        gSpecialVar_Result = SOCIAL_LINK_BIND_OK;
    else
        gSpecialVar_Result = SOCIAL_LINK_BIND_FAILED;
}

// Binds the first Pokémon of the given species found in the party, then the PC, that isn't bound to a link yet.
// Use this after catching a scripted encounter, since the caught Pokémon may have been sent to the PC.
void ScrCmd_sociallink_bindspecies(struct ScriptContext *ctx)
{
    u32 linkId = VarGet(ScriptReadHalfword(ctx));
    enum Species species = VarGet(ScriptReadHalfword(ctx));

    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE);
    Script_RequestWriteVar(VAR_RESULT);
    gSpecialVar_Result = SOCIAL_LINK_BIND_FAILED;

    for (u32 slot = 0; slot < PARTY_SIZE; slot++)
    {
        struct BoxPokemon *boxMon = &gParties[B_TRAINER_PLAYER][slot].box;
        if (GetBoxMonData(boxMon, MON_DATA_SPECIES) == species && SocialLink_BindBoxMon(linkId, boxMon))
        {
            gSpecialVar_Result = SOCIAL_LINK_BIND_OK;
            return;
        }
    }

    for (u32 box = 0; box < TOTAL_BOXES_COUNT; box++)
    {
        for (u32 pos = 0; pos < IN_BOX_COUNT; pos++)
        {
            struct BoxPokemon *boxMon = GetBoxedMonPtr(box, pos);
            if (GetBoxMonData(boxMon, MON_DATA_SPECIES) == species && SocialLink_BindBoxMon(linkId, boxMon))
            {
                gSpecialVar_Result = SOCIAL_LINK_BIND_OK;
                return;
            }
        }
    }
}

// Sets VAR_RESULT to the party slot of the Pokémon bound to linkId, or PARTY_SIZE if it isn't in the party.
void ScrCmd_sociallink_getpartyslot(struct ScriptContext *ctx)
{
    u32 linkId = VarGet(ScriptReadHalfword(ctx));

    Script_RequestEffects(SCREFF_V1);
    Script_RequestWriteVar(VAR_RESULT);
    gSpecialVar_Result = PARTY_SIZE;

    for (u32 slot = 0; slot < PARTY_SIZE; slot++)
    {
        if (SocialLink_GetLinkForBoxMon(&gParties[B_TRAINER_PLAYER][slot].box) == (s32)linkId)
        {
            gSpecialVar_Result = slot;
            return;
        }
    }
}
