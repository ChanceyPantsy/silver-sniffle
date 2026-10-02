#include "global.h"
#include "social_links.h"
#include "config/social_links.h"
#include "test/battle.h"

#define BIND_PLAYER_MON(linkId, slot) SocialLink_BindBoxMon(linkId, &gBattleTestRunnerState->data.recordedBattle.parties[B_TRAINER_PLAYER][slot].box)

WILD_BATTLE_TEST("(Social Links) A linked Pokémon earns points for winning a battle without fainting")
{
    GIVEN {
        ResetSocialLinks();
        PLAYER(SPECIES_WOBBUFFET) { Level(100); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_CATERPIE) { Level(5); Moves(MOVE_CELEBRATE); }
        BIND_PLAYER_MON(SOCIAL_LINK_STARTER, 0);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(gBattleOutcome, B_OUTCOME_WON);
        EXPECT_EQ(SocialLink_GetPoints(SOCIAL_LINK_STARTER), SOCIAL_LINK_POINTS_BATTLE + SOCIAL_LINK_POINTS_NO_FAINT);
    }
}

AI_SINGLE_BATTLE_TEST("(Social Links) Trainer battles give linked Pokémon bonus points")
{
    GIVEN {
        ResetSocialLinks();
        PLAYER(SPECIES_WOBBUFFET) { Level(100); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_CATERPIE) { Level(5); Moves(MOVE_CELEBRATE); }
        BIND_PLAYER_MON(SOCIAL_LINK_STARTER, 0);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } THEN {
        EXPECT_EQ(SocialLink_GetPoints(SOCIAL_LINK_STARTER), SOCIAL_LINK_POINTS_BATTLE + SOCIAL_LINK_POINTS_TRAINER_BONUS + SOCIAL_LINK_POINTS_NO_FAINT);
    }
}

AI_SINGLE_BATTLE_TEST("(Social Links) A linked Pokémon that faints loses the no-faint bonus")
{
    GIVEN {
        ResetSocialLinks();
        PLAYER(SPECIES_WOBBUFFET) { Level(100); HP(1); Speed(1); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Level(100); Nature(NATURE_ADAMANT); Speed(100); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_CATERPIE) { Level(5); Speed(50); Moves(MOVE_TACKLE); }
        BIND_PLAYER_MON(SOCIAL_LINK_STARTER, 0);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); SEND_OUT(player, 1); }
        TURN { MOVE(player, MOVE_TACKLE); }
    } THEN {
        EXPECT_EQ(gBattleOutcome, B_OUTCOME_WON);
        EXPECT_EQ(SocialLink_GetPoints(SOCIAL_LINK_STARTER), SOCIAL_LINK_POINTS_BATTLE + SOCIAL_LINK_POINTS_TRAINER_BONUS);
    }
}

WILD_BATTLE_TEST("(Social Links) A linked Pokémon earns points for each level gained in battle")
{
    GIVEN {
        ResetSocialLinks();
        FLAG_SET(FLAG_IS_CHAMPION); // Lift the rank cap so the points aren't clamped.
        PLAYER(SPECIES_WOBBUFFET) { Level(5); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_CATERPIE) { Level(20); HP(1); Moves(MOVE_CELEBRATE); }
        BIND_PLAYER_MON(SOCIAL_LINK_STARTER, 0);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        u32 levelsGained = GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL) - 5;
        EXPECT_GT(levelsGained, 0);
        EXPECT_EQ(SocialLink_GetPoints(SOCIAL_LINK_STARTER),
                  SOCIAL_LINK_POINTS_BATTLE + SOCIAL_LINK_POINTS_NO_FAINT + levelsGained * SOCIAL_LINK_POINTS_LEVEL_UP);
    }
}

WILD_BATTLE_TEST("(Social Links) Unlinked party members don't affect links")
{
    GIVEN {
        ResetSocialLinks();
        PLAYER(SPECIES_WOBBUFFET) { Level(100); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_CATERPIE) { Level(5); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        for (u32 linkId = 0; linkId < NUM_SOCIAL_LINKS; linkId++)
            EXPECT_EQ(SocialLink_GetPoints(linkId), 0);
    }
}
