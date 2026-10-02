#include "global.h"
#include "event_data.h"
#include "pokemon.h"
#include "social_links.h"
#include "config/social_links.h"
#include "test/overworld_script.h"
#include "test/test.h"

static void ResetSocialLinkTestState(void)
{
    ResetSocialLinks();
    FlagClear(FLAG_DEFEATED_RUSTBORO_GYM);
    FlagClear(FLAG_BADGE01_GET);
    FlagClear(FLAG_IS_CHAMPION);
    ZeroPlayerPartyMons();
}

TEST("(Social Links) Ranks are derived from points")
{
    ResetSocialLinkTestState();
    SocialLink_SetRank(SOCIAL_LINK_ROXANNE, 4);
    EXPECT_EQ(SocialLink_GetRank(SOCIAL_LINK_ROXANNE), 4);
    EXPECT_EQ(SocialLink_GetPoints(SOCIAL_LINK_ROXANNE), SocialLink_GetPointsForRank(4));
    SocialLink_SetRank(SOCIAL_LINK_ROXANNE, SOCIAL_LINK_MAX_RANK + 5);
    EXPECT_EQ(SocialLink_GetRank(SOCIAL_LINK_ROXANNE), SOCIAL_LINK_MAX_RANK);
}

TEST("(Social Links) Points stop at the rank cap until its gate flag is set")
{
    ResetSocialLinkTestState();

    EXPECT_EQ(SocialLink_GetRankCap(SOCIAL_LINK_ROXANNE), 3);
    SocialLink_AddPoints(SOCIAL_LINK_ROXANNE, 1000);
    EXPECT_EQ(SocialLink_GetRank(SOCIAL_LINK_ROXANNE), 3);
    EXPECT_EQ(SocialLink_GetPoints(SOCIAL_LINK_ROXANNE), SocialLink_GetPointsForRank(3));

    FlagSet(FLAG_DEFEATED_RUSTBORO_GYM);
    EXPECT_EQ(SocialLink_GetRankCap(SOCIAL_LINK_ROXANNE), 7);
    EXPECT(SocialLink_AddPoints(SOCIAL_LINK_ROXANNE, 1000));
    EXPECT_EQ(SocialLink_GetRank(SOCIAL_LINK_ROXANNE), 7);

    FlagSet(FLAG_IS_CHAMPION);
    SocialLink_AddPoints(SOCIAL_LINK_ROXANNE, 1000);
    EXPECT_EQ(SocialLink_GetRank(SOCIAL_LINK_ROXANNE), SOCIAL_LINK_MAX_RANK);
    ResetSocialLinkTestState();
}

TEST("(Social Links) AddPoints only reports a rank up when the rank changes")
{
    ResetSocialLinkTestState();
    EXPECT(!SocialLink_AddPoints(SOCIAL_LINK_ROXANNE, SocialLink_GetPointsForRank(1) - 1));
    EXPECT(SocialLink_AddPoints(SOCIAL_LINK_ROXANNE, 1));
    EXPECT_EQ(SocialLink_GetRank(SOCIAL_LINK_ROXANNE), 1);
}

TEST("(Social Links) Pokémon links gain nothing until bound")
{
    ResetSocialLinkTestState();
    EXPECT(!SocialLink_AddPoints(SOCIAL_LINK_STARTER, 20));
    EXPECT_EQ(SocialLink_GetPoints(SOCIAL_LINK_STARTER), 0);
    EXPECT_EQ(SocialLink_GetType(SOCIAL_LINK_STARTER), TYPE_NONE);
}

TEST("(Social Links) sociallink_bindpartymon ties a link to a Pokémon and its primary type")
{
    ResetSocialLinkTestState();
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_MUDKIP, 5;
        givemon SPECIES_TREECKO, 5;
        sociallink_bindpartymon SOCIAL_LINK_STARTER, 0;
    );
    EXPECT_EQ(gSpecialVar_Result, SOCIAL_LINK_BIND_OK);
    EXPECT(SocialLink_IsMonBound(SOCIAL_LINK_STARTER));
    EXPECT_EQ(SocialLink_GetType(SOCIAL_LINK_STARTER), GetSpeciesType(SPECIES_MUDKIP, 0));
    EXPECT_EQ(SocialLink_GetLinkForBoxMon(&gParties[B_TRAINER_PLAYER][0].box), SOCIAL_LINK_STARTER);
    EXPECT_EQ(SocialLink_GetLinkForBoxMon(&gParties[B_TRAINER_PLAYER][1].box), -1);

    // A link can't be rebound, and a Pokémon can't hold two links.
    RUN_OVERWORLD_SCRIPT(sociallink_bindpartymon SOCIAL_LINK_STARTER, 1);
    EXPECT_EQ(gSpecialVar_Result, SOCIAL_LINK_BIND_FAILED);
    RUN_OVERWORLD_SCRIPT(sociallink_bindpartymon SOCIAL_LINK_EVENT_MON_1, 0);
    EXPECT_EQ(gSpecialVar_Result, SOCIAL_LINK_BIND_FAILED);

    RUN_OVERWORLD_SCRIPT(sociallink_getpartyslot SOCIAL_LINK_STARTER);
    EXPECT_EQ(gSpecialVar_Result, 0);
    ResetSocialLinkTestState();
}

TEST("(Social Links) sociallink_bindspecies finds the first unlinked Pokémon of a species")
{
    ResetSocialLinkTestState();
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_PIKACHU, 5;
        givemon SPECIES_GROUDON, 70;
        sociallink_bindspecies SOCIAL_LINK_EVENT_MON_1, SPECIES_GROUDON;
    );
    EXPECT_EQ(gSpecialVar_Result, SOCIAL_LINK_BIND_OK);
    EXPECT_EQ(SocialLink_GetLinkForBoxMon(&gParties[B_TRAINER_PLAYER][1].box), SOCIAL_LINK_EVENT_MON_1);

    RUN_OVERWORLD_SCRIPT(sociallink_bindspecies SOCIAL_LINK_EVENT_MON_2, SPECIES_KYOGRE);
    EXPECT_EQ(gSpecialVar_Result, SOCIAL_LINK_BIND_FAILED);
    ResetSocialLinkTestState();
}

TEST("(Social Links) sociallink_addpoints and sociallink_getrank work from scripts")
{
    ResetSocialLinkTestState();
    RUN_OVERWORLD_SCRIPT(
        sociallink_addpoints SOCIAL_LINK_STEVEN, 30;
    );
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    RUN_OVERWORLD_SCRIPT(sociallink_getrank SOCIAL_LINK_STEVEN);
    EXPECT_EQ(gSpecialVar_Result, 2);
    RUN_OVERWORLD_SCRIPT(sociallink_getrankcap SOCIAL_LINK_STEVEN);
    EXPECT_EQ(gSpecialVar_Result, 2);
    ResetSocialLinkTestState();
}

TEST("(Social Links) Maxed links only add shiny rolls for Pokémon sharing their type")
{
    ResetSocialLinkTestState();
    ASSUME(GetSpeciesType(SPECIES_GEODUDE, 0) == TYPE_ROCK);
    ASSUME(GetSpeciesType(SPECIES_PIKACHU, 0) == TYPE_ELECTRIC);
    ASSUME(GetSpeciesType(SPECIES_PIKACHU, 1) == TYPE_ELECTRIC);

    SocialLink_SetRank(SOCIAL_LINK_ROXANNE, SOCIAL_LINK_MAX_RANK - 1);
    EXPECT_EQ(SocialLink_GetShinyRollsForSpecies(SPECIES_GEODUDE), 0);

    SocialLink_SetRank(SOCIAL_LINK_ROXANNE, SOCIAL_LINK_MAX_RANK);
    EXPECT_EQ(SocialLink_GetShinyRollsForSpecies(SPECIES_GEODUDE), SOCIAL_LINK_MAXED_SHINY_ROLLS);
    EXPECT_EQ(SocialLink_GetShinyRollsForSpecies(SPECIES_PIKACHU), 0);

    // Matches the secondary type too, and stacks across links.
    ASSUME(GetSpeciesType(SPECIES_NOSEPASS, 0) == TYPE_ROCK);
    ASSUME(GetSpeciesType(SPECIES_MAGNEMITE, 1) == TYPE_STEEL);
    SocialLink_SetRank(SOCIAL_LINK_WATTSON, SOCIAL_LINK_MAX_RANK);
    SocialLink_SetRank(SOCIAL_LINK_STEVEN, SOCIAL_LINK_MAX_RANK);
    EXPECT_EQ(SocialLink_GetShinyRollsForSpecies(SPECIES_MAGNEMITE), SOCIAL_LINK_MAXED_SHINY_ROLLS * 2);
    ResetSocialLinkTestState();
}

TEST("(Social Links) A maxed Pokémon link boosts its own type")
{
    ResetSocialLinkTestState();
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_TORCHIC, 5;
        sociallink_bindpartymon SOCIAL_LINK_STARTER, 0;
    );
    SocialLink_SetRank(SOCIAL_LINK_STARTER, SOCIAL_LINK_MAX_RANK);
    EXPECT_EQ(SocialLink_GetShinyRollsForSpecies(SPECIES_VULPIX), SOCIAL_LINK_MAXED_SHINY_ROLLS);
    EXPECT_EQ(SocialLink_GetShinyRollsForSpecies(SPECIES_MUDKIP), 0);
    ResetSocialLinkTestState();
}

TEST("(Social Links) Fae is a Ground-type NPC link that starts capped at rank 2")
{
    ResetSocialLinkTestState();
    EXPECT_EQ(gSocialLinksInfo[SOCIAL_LINK_FAE].kind, SOCIAL_LINK_KIND_NPC);
    EXPECT_EQ(SocialLink_GetType(SOCIAL_LINK_FAE), TYPE_GROUND);
    EXPECT_EQ(SocialLink_GetRankCap(SOCIAL_LINK_FAE), 2);

    FlagSet(FLAG_BADGE01_GET);
    EXPECT_EQ(SocialLink_GetRankCap(SOCIAL_LINK_FAE), 3);
    ResetSocialLinkTestState();
}
