#ifndef GUARD_SOCIAL_LINKS_H
#define GUARD_SOCIAL_LINKS_H

#include "constants/social_links.h"

struct ScriptContext;

// The highest rank a link can reach while gateFlag is set. A link's cap is the highest
// rankCap among its gates whose flag is set, or its baseRankCap if none are.
struct SocialLinkRankGate
{
    u16 gateFlag;
    u8 rankCap;
};

struct SocialLinkInfo
{
    const u8 *name;
    u8 kind;                // SOCIAL_LINK_KIND_*
    enum Type type;         // NPC links only. Pokémon links use the bound Pokémon's type.
    u8 baseRankCap;
    const struct SocialLinkRankGate *gates; // Terminated by a gate with gateFlag == 0.
};

extern const struct SocialLinkInfo gSocialLinksInfo[NUM_SOCIAL_LINKS];

void ResetSocialLinks(void);
const u8 *SocialLink_GetName(u32 linkId);
enum Type SocialLink_GetType(u32 linkId);
u32 SocialLink_GetPoints(u32 linkId);
u32 SocialLink_GetRank(u32 linkId);
u32 SocialLink_GetRankCap(u32 linkId);
u32 SocialLink_GetPointsForRank(u32 rank);
bool32 SocialLink_AddPoints(u32 linkId, u32 points);
void SocialLink_SetRank(u32 linkId, u32 rank);
bool32 SocialLink_IsMonBound(u32 linkId);
bool32 SocialLink_BindBoxMon(u32 linkId, struct BoxPokemon *boxMon);
s32 SocialLink_GetLinkForBoxMon(struct BoxPokemon *boxMon);
u32 SocialLink_GetShinyRollsForSpecies(enum Species species);

void SocialLink_OnBattleStart(void);
void SocialLink_OnBattleEnd(void);

// callnative script commands, see asm/macros/event.inc
void ScrCmd_sociallink_addpoints(struct ScriptContext *ctx);
void ScrCmd_sociallink_getrank(struct ScriptContext *ctx);
void ScrCmd_sociallink_getrankcap(struct ScriptContext *ctx);
void ScrCmd_sociallink_setrank(struct ScriptContext *ctx);
void ScrCmd_sociallink_bindpartymon(struct ScriptContext *ctx);
void ScrCmd_sociallink_bindspecies(struct ScriptContext *ctx);
void ScrCmd_sociallink_getpartyslot(struct ScriptContext *ctx);

#endif // GUARD_SOCIAL_LINKS_H
