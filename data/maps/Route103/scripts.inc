Route103_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route103_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, Route103_OnLoad
	.byte 0

Route103_OnTransition:
	call Common_EventScript_SetupRivalGfxId
	call ProfBirch_EventScript_UpdateLocation
	end

Route103_OnLoad:
	call_if_set FLAG_SYS_GAME_CLEAR, Route103_EventScript_OpenAlteringCave
	end

Route103_EventScript_OpenAlteringCave::
	setmetatile 45, 5, METATILE_General_CaveEntrance_Top, TRUE
	setmetatile 45, 6, METATILE_General_CaveEntrance_Bottom, FALSE
	return

Route103_EventScript_Rival::
	lockall
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, Route103_EventScript_RivalMay
	goto_if_eq VAR_RESULT, FEMALE, Route103_EventScript_RivalBrendan
	end

Route103_EventScript_RivalMay::
	msgbox Route103_Text_MayRoute103Pokemon, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_MAY, TRUE
	applymovement LOCALID_ROUTE103_RIVAL, Common_Movement_FacePlayer
	waitmovement 0
	applymovement LOCALID_ROUTE103_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_ROUTE103_RIVAL, Common_Movement_Delay48
	waitmovement 0
	msgbox Route103_Text_MayLetsBattle, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 0, Route103_EventScript_StartMayBattleTreecko
	case 1, Route103_EventScript_StartMayBattleTorchic
	case 2, Route103_EventScript_StartMayBattleMudkip
	end

Route103_EventScript_RivalBrendan::
	msgbox Route103_Text_BrendanRoute103Pokemon, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	applymovement LOCALID_ROUTE103_RIVAL, Common_Movement_FacePlayer
	waitmovement 0
	applymovement LOCALID_ROUTE103_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_ROUTE103_RIVAL, Common_Movement_Delay48
	waitmovement 0
	msgbox Route103_Text_BrendanLetsBattle, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 0, Route103_EventScript_StartBrendanBattleTreecko
	case 1, Route103_EventScript_StartBrendanBattleTorchic
	case 2, Route103_EventScript_StartBrendanBattleMudkip
	end

Route103_EventScript_StartMayBattleTreecko::
	trainerbattle_no_intro TRAINER_MAY_ROUTE_103_TREECKO, Route103_Text_MayDefeated
	goto Route103_EventScript_AfterMayBattle
	end

Route103_EventScript_StartMayBattleTorchic::
	trainerbattle_no_intro TRAINER_MAY_ROUTE_103_TORCHIC, Route103_Text_MayDefeated
	goto Route103_EventScript_AfterMayBattle
	end

Route103_EventScript_StartMayBattleMudkip::
	trainerbattle_no_intro TRAINER_MAY_ROUTE_103_MUDKIP, Route103_Text_MayDefeated
	goto Route103_EventScript_AfterMayBattle
	end

Route103_EventScript_StartBrendanBattleTreecko::
	trainerbattle_no_intro TRAINER_BRENDAN_ROUTE_103_TREECKO, Route103_Text_BrendanDefeated
	goto Route103_EventScript_AfterBrendanBattle
	end

Route103_EventScript_StartBrendanBattleTorchic::
	trainerbattle_no_intro TRAINER_BRENDAN_ROUTE_103_TORCHIC, Route103_Text_BrendanDefeated
	goto Route103_EventScript_AfterBrendanBattle
	end

Route103_EventScript_StartBrendanBattleMudkip::
	trainerbattle_no_intro TRAINER_BRENDAN_ROUTE_103_MUDKIP, Route103_Text_BrendanDefeated
	goto Route103_EventScript_AfterBrendanBattle
	end

Route103_EventScript_AfterMayBattle::
	msgbox Route103_Text_MayTimeToHeadBack, MSGBOX_DEFAULT
	goto Route103_EventScript_RivalExit
	end

Route103_EventScript_AfterBrendanBattle::
	msgbox Route103_Text_BrendanTimeToHeadBack, MSGBOX_DEFAULT
	goto Route103_EventScript_RivalExit
	end

Route103_EventScript_RivalExit::
	closemessage
	switch VAR_FACING
	case DIR_SOUTH, Route103_EventScript_RivalExitFacingSouth
	case DIR_NORTH, Route103_EventScript_RivalExitFacingNorth
	case DIR_WEST, Route103_EventScript_RivalExitFacingEastOrWest
	case DIR_EAST, Route103_EventScript_RivalExitFacingEastOrWest
	end

Route103_EventScript_RivalExitFacingNorth::
	applymovement LOCALID_PLAYER, Route103_Movement_WatchRivalExitFacingNorth
	applymovement LOCALID_ROUTE103_RIVAL, Route103_Movement_RivalExitFacingNorth1
	waitmovement 0
	playse SE_LEDGE
	applymovement LOCALID_ROUTE103_RIVAL, Route103_Movement_RivalExitFacingNorth2
	waitmovement 0
	goto Route103_EventScript_RivalEnd
	end

Route103_EventScript_RivalExitFacingEastOrWest::
	applymovement LOCALID_PLAYER, Route103_Movement_WatchRivalExitFacingEastOrWest
	applymovement LOCALID_ROUTE103_RIVAL, Route103_Movement_RivalExit1
	waitmovement 0
	playse SE_LEDGE
	applymovement LOCALID_ROUTE103_RIVAL, Route103_Movement_RivalExit2
	waitmovement 0
	goto Route103_EventScript_RivalEnd
	end

Route103_EventScript_RivalExitFacingSouth::
	applymovement LOCALID_ROUTE103_RIVAL, Route103_Movement_RivalExit1
	waitmovement 0
	playse SE_LEDGE
	applymovement LOCALID_ROUTE103_RIVAL, Route103_Movement_RivalExit2
	waitmovement 0
	goto Route103_EventScript_RivalEnd
	end

Route103_EventScript_RivalEnd::
	removeobject LOCALID_ROUTE103_RIVAL
	setvar VAR_BIRCH_LAB_STATE, 4
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BIRCHS_LAB_RIVAL
	setflag FLAG_DEFEATED_RIVAL_ROUTE103
	setvar VAR_OLDALE_RIVAL_STATE, 1
	clearflag FLAG_HIDE_OLDALE_TOWN_RIVAL
	savebgm MUS_DUMMY
	fadedefaultbgm
	releaseall
	end

Route103_Movement_RivalExitFacingNorth1:
	walk_left
	walk_down
	step_end

Route103_Movement_RivalExitFacingNorth2:
	jump_2_down
	delay_16
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Route103_Movement_WatchRivalExitFacingNorth:
	delay_16
	delay_4
	walk_in_place_faster_left
	delay_16
	walk_in_place_faster_down
	step_end

Route103_Movement_RivalExit1:
	walk_down
	step_end

Route103_Movement_RivalExit2:
	jump_2_down
	delay_16
	walk_down
	walk_down
	walk_down
	step_end

Route103_Movement_WatchRivalExitFacingEastOrWest:
	delay_16
	walk_in_place_faster_down
	step_end

Route103_EventScript_Boy::
	msgbox Route103_Text_ShouldHaveBroughtPotion, MSGBOX_NPC
	end

Route103_EventScript_Man::
	msgbox Route103_Text_ShortcutToOldale, MSGBOX_NPC
	end

Route103_EventScript_RouteSign::
	msgbox Route103_Text_RouteSign, MSGBOX_SIGN
	end

Route103_EventScript_Daisy::
	trainerbattle_single TRAINER_DAISY, Route103_Text_DaisyIntro, Route103_Text_DaisyDefeated
	msgbox Route103_Text_DaisyPostBattle, MSGBOX_AUTOCLOSE
	end

Route103_EventScript_Amy::
	trainerbattle_double TRAINER_AMY_AND_LIV_1, Route103_Text_AmyIntro, Route103_Text_AmyDefeated, Route103_Text_AmyNotEnoughPokemon, Route102_EventScript_AmyRegisterMatchCallAfterBattle
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route102_EventScript_AmyRematch
	msgbox Route103_Text_AmyPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_AmyRegisterMatchCallAfterBattle::
	msgbox Route103_Text_AmyLivRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_AMY_AND_LIV_1
	release
	end

Route102_EventScript_AmyRematch::
	trainerbattle_rematch_double TRAINER_AMY_AND_LIV_1, Route103_Text_AmyRematchIntro, Route103_Text_AmyRematchDefeated, Route103_Text_AmyRematchNotEnoughPokemon
	msgbox Route103_Text_AmyRematchPostBattle, MSGBOX_AUTOCLOSE
	end

Route103_EventScript_Liv::
	trainerbattle_double TRAINER_AMY_AND_LIV_1, Route103_Text_LivIntro, Route103_Text_LivDefeated, Route103_Text_LivNotEnoughPokemon, Route102_EventScript_LivRegisterMatchCallAfterBattle
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route102_EventScript_LivRematch
	msgbox Route103_Text_LivPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_LivRegisterMatchCallAfterBattle::
	msgbox Route103_Text_AmyLivRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_AMY_AND_LIV_1
	release
	end

Route102_EventScript_LivRematch::
	trainerbattle_rematch_double TRAINER_AMY_AND_LIV_1, Route103_Text_LivRematchIntro, Route103_Text_LivRematchDefeated, Route103_Text_LivRematchNotEnoughPokemon
	msgbox Route103_Text_LivRematchPostBattle, MSGBOX_AUTOCLOSE
	end

Route103_EventScript_Andrew::
	trainerbattle_single TRAINER_ANDREW, Route103_Text_AndrewIntro, Route103_Text_AndrewDefeated
	msgbox Route103_Text_AndrewPostBattle, MSGBOX_AUTOCLOSE
	end

Route103_EventScript_Miguel::
	trainerbattle_single TRAINER_MIGUEL_1, Route103_Text_MiguelIntro, Route103_Text_MiguelDefeated, Route102_EventScript_MiguelRegisterMatchCallAfterBattle
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route103_EventScript_MiguelRematch
	msgbox Route103_Text_MiguelPostBattle, MSGBOX_DEFAULT
	release
	end

Route102_EventScript_MiguelRegisterMatchCallAfterBattle::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox Route103_Text_MiguelRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_MIGUEL_1
	release
	end

Route103_EventScript_MiguelRematch::
	trainerbattle_rematch TRAINER_MIGUEL_1, Route103_Text_MiguelRematchIntro, Route103_Text_MiguelRematchDefeated
	msgbox Route103_Text_MiguelRematchPostBattle, MSGBOX_AUTOCLOSE
	end

Route103_EventScript_Marcos::
	trainerbattle_single TRAINER_MARCOS, Route103_Text_MarcosIntro, Route103_Text_MarcosDefeated
	msgbox Route103_Text_MarcosPostBattle, MSGBOX_AUTOCLOSE
	end

Route103_EventScript_Rhett::
	trainerbattle_single TRAINER_RHETT, Route103_Text_RhettIntro, Route103_Text_RhettDefeated
	msgbox Route103_Text_RhettPostBattle, MSGBOX_AUTOCLOSE
	end

Route103_EventScript_Pete::
	trainerbattle_single TRAINER_PETE, Route103_Text_PeteIntro, Route103_Text_PeteDefeated
	msgbox Route103_Text_PetePostBattle, MSGBOX_AUTOCLOSE
	end

Route103_EventScript_Isabelle::
	trainerbattle_single TRAINER_ISABELLE, Route103_Text_IsabelleIntro, Route103_Text_IsabelleDefeated
	msgbox Route103_Text_IsabellePostBattle, MSGBOX_AUTOCLOSE
	end

Route103_Text_MayRoute103Pokemon:
	.string "MAY: Let's see… The POKéMON found\n"
	.string "on ROUTE 103 include…$"

Route103_Text_MayLetsBattle:
	.string "Oh, hi, {PLAYER}{KUN}!\p"
	.string "…Oh, I see, my dad gave you\n"
	.string "a POKéMON as a gift.\p"
	.string "Since we're here, let's have a quick\n"
	.string "battle!\p"
	.string "I'll give you a taste of what being\n"
	.string "a TRAINER is like.$"

Route103_Text_MayDefeated:
	.string "Wow! That's great!\n"
	.string "{PLAYER}{KUN}, you're pretty good!$"

Route103_Text_MayTimeToHeadBack:
	.string "MAY: I think I know why my dad has\n"
	.string "an eye out for you now.\p"
	.string "I mean, you just got that POKéMON,\n"
	.string "but it already likes you.\p"
	.string "You might be able to befriend any\n"
	.string "kind of POKéMON easily.\p"
	.string "Well, it's time to head back to\n"
	.string "the LAB.$"

Route103_Text_BrendanRoute103Pokemon:
	.string "BRENDAN: Okay, so it's this one and\n"
	.string "that one that live on ROUTE 103…$"

Route103_Text_BrendanLetsBattle:
	.string "Hey, it's {PLAYER}!\p"
	.string "…Oh, yeah, Dad gave you a POKéMON.\p"
	.string "Since we're here, how about a little\n"
	.string "battle?\p"
	.string "I'll teach you what being a TRAINER's\n"
	.string "about!$"

Route103_Text_BrendanDefeated:
	.string "Huh, {PLAYER}, you're not too shabby.$"

Route103_Text_BrendanTimeToHeadBack:
	.string "BRENDAN: I think I get it.\n"
	.string "I think I know why my dad has his eye\l"
	.string "out for you now.\p"
	.string "Look, your POKéMON already likes you,\n"
	.string "even though you just got it.\p"
	.string "{PLAYER}, I get the feeling that you\n"
	.string "could befriend any POKéMON with ease.\p"
	.string "We should head back to the LAB.$"

Route103_Text_ShouldHaveBroughtPotion:
	.string "My POKéMON is staggeringly tired…\n"
	.string "I should have brought a POTION…$"

Route103_Text_ShortcutToOldale:
	.string "If you cross the sea from here,\n"
	.string "it'll be a shortcut to OLDALE TOWN.\p"
	.string "Fufufu, that's useful, isn't it?$"

Route103_Text_RouteSign:
	.string "ROUTE 103\n"
	.string "{DOWN_ARROW} OLDALE TOWN$"


@ Fae: granddaughter of the Mt. Pyre orb keepers. See SOCIAL_LINK_FAE.

Route103_EventScript_FaeIntroTrigger::
	lockall
	playse SE_PIN
	applymovement LOCALID_ROUTE103_FAE, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_ROUTE103_FAE, Common_Movement_FaceDown
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	call Route103_EventScript_FaeIntro
	releaseall
	end

Route103_EventScript_FaeIntro::
	setspeaker SP_NAME_FAE
	msgbox Route103_Text_FaeCallsOut, MSGBOX_DEFAULT
	closemessage
	playmoncry SPECIES_POOCHYENA, CRY_MODE_ENCOUNTER
	waitmoncry
	setspeaker SP_NAME_FAE
	msgbox Route103_Text_FaeTheyreHere, MSGBOX_DEFAULT
	closemessage
	special ZeroEnemyPartyMons
	createmon 1, 0, SPECIES_POOCHYENA, 3
	createmon 1, 1, SPECIES_ZIGZAGOON, 3
	special SavePlayerParty
	setmultitrainerbattle TRAINER_NONE, NULL, TRAINER_NONE, NULL, PARTNER_FAE
	multi_do_fixed MULTI_BATTLE_2_VS_WILD
	setvar VAR_FAE_STATE, FAE_STATE_MET_ROUTE_103
	sociallink_setrank SOCIAL_LINK_FAE, 1
	setspeaker SP_NAME_FAE
	msgbox Route103_Text_FaeIntroduction, MSGBOX_DEFAULT
	setspeaker SP_NAME_NONE
	giveitem ITEM_POTION, 2
	setspeaker SP_NAME_FAE
	msgbox Route103_Text_FaeComeFindMe, MSGBOX_DEFAULT
	return

Route103_EventScript_Fae::
	lock
	faceplayer
	goto_if_eq VAR_FAE_STATE, FAE_STATE_NOT_MET, Route103_EventScript_FaeIntroByTalking
	sociallink_getrank SOCIAL_LINK_FAE
	copyvar VAR_0x8004, VAR_RESULT
	sociallink_getrankcap SOCIAL_LINK_FAE
	goto_if_ge VAR_0x8004, VAR_RESULT, Route103_EventScript_FaeAtRankCap
	setspeaker SP_NAME_FAE
	msgbox Route103_Text_FaeHangOut, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route103_EventScript_FaeDeclineHangOut
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	delay 30
	fadescreenswapbuffers FADE_FROM_BLACK
	setspeaker SP_NAME_FAE
	call_if_le VAR_0x8004, 1, Route103_EventScript_FaeScene_Orbs
	call_if_ge VAR_0x8004, 2, Route103_EventScript_FaeScene_Chingling
	sociallink_addpoints SOCIAL_LINK_FAE, 8
	call_if_eq VAR_RESULT, TRUE, Route103_EventScript_FaeRankUp
	release
	end

Route103_EventScript_FaeIntroByTalking::
	call Route103_EventScript_FaeIntro
	release
	end

Route103_EventScript_FaeDeclineHangOut::
	setspeaker SP_NAME_FAE
	msgbox Route103_Text_FaeDeclined, MSGBOX_DEFAULT
	release
	end

Route103_EventScript_FaeAtRankCap::
	setspeaker SP_NAME_FAE
	msgbox Route103_Text_FaeGoAdventure, MSGBOX_DEFAULT
	release
	end

Route103_EventScript_FaeScene_Orbs::
	msgbox Route103_Text_FaeSceneOrbs, MSGBOX_DEFAULT
	return

Route103_EventScript_FaeScene_Chingling::
	msgbox Route103_Text_FaeSceneChingling, MSGBOX_DEFAULT
	return

@ Fae gives a small gift the first time her link reaches certain ranks.
Route103_EventScript_FaeRankUp::
	sociallink_getrank SOCIAL_LINK_FAE
	copyvar VAR_0x8005, VAR_RESULT
	buffernumberstring STR_VAR_1, VAR_0x8005
	setspeaker SP_NAME_NONE
	playfanfare MUS_LEVEL_UP
	msgbox Route103_Text_FaeBondGrew, MSGBOX_DEFAULT
	waitfanfare
	goto_if_eq VAR_0x8005, 2, Route103_EventScript_FaeGiftRank2
	return

Route103_EventScript_FaeGiftRank2::
	setspeaker SP_NAME_FAE
	msgbox Route103_Text_FaeGiftSootheBell, MSGBOX_DEFAULT
	setspeaker SP_NAME_NONE
	giveitem ITEM_SOOTHE_BELL
	return

Route103_Text_FaeCallsOut:
	.string "Hey! You, with the Pokémon!\n"
	.string "Don't move… Something's in\l"
	.string "the grass behind you!$"

Route103_Text_FaeTheyreHere:
	.string "Two of them! I can't handle both\n"
	.string "on my own. Fight with me, okay?$"

Route103_Text_FaeIntroduction:
	.string "Whew… We make a good team!\p"
	.string "I'm FAE. I'm staying with my aunt\n"
	.string "in OLDALE for a while.\p"
	.string "My grandparents look after MT. PYRE,\n"
	.string "way out past the city of LILYCOVE.\p"
	.string "Grandpa says a keeper should know\n"
	.string "the land she's meant to protect,\l"
	.string "so here I am, learning!\p"
	.string "You're the new kid in LITTLEROOT,\n"
	.string "right? Thanks for helping me.\l"
	.string "Here, take these!$"

Route103_Text_FaeComeFindMe:
	.string "I'll be around here, training with\n"
	.string "CHINGLING. Come talk anytime!\p"
	.string "And {PLAYER}… I've got a good\n"
	.string "feeling about you. I mean it!$"

Route103_Text_FaeHangOut:
	.string "Hi, {PLAYER}! Want to sit and\n"
	.string "talk for a bit?$"

Route103_Text_FaeDeclined:
	.string "That's okay! Go show them what\n"
	.string "you can do. I'm cheering for you!$"

Route103_Text_FaeGoAdventure:
	.string "I feel like we've talked about\n"
	.string "everything for now!\p"
	.string "Go out there and make some new\n"
	.string "stories. Then come tell me all\l"
	.string "about them, okay?$"

Route103_Text_FaeSceneOrbs:
	.string "When I was little, Grandma told me\n"
	.string "a story every night.\p"
	.string "Long ago, a giant of the land and\n"
	.string "a giant of the sea fought until\l"
	.string "the whole world was ending.\p"
	.string "Two orbs calmed them down. One red,\n"
	.string "one blue. They're kept at the top\l"
	.string "of MT. PYRE, even now.\p"
	.string "I used to think it was just a\n"
	.string "bedtime story…\p"
	.string "But Grandpa never smiles when he\n"
	.string "talks about it. Isn't that weird?$"

Route103_Text_FaeSceneChingling:
	.string "CHINGLING hatched from an Egg\n"
	.string "Grandma found at the summit.\p"
	.string "The CHIMECHO up there ring every\n"
	.string "evening. Grandma says the sound\l"
	.string "soothes the spirits on the mountain.\p"
	.string "Someday CHINGLING will ring like\n"
	.string "that too. I just know it!\p"
	.string "…Hey, {PLAYER}. Thanks for always\n"
	.string "listening to me ramble.$"

Route103_Text_FaeBondGrew:
	.string "Your bond with FAE grew stronger!\n"
	.string "(Rank {STR_VAR_1})$"

Route103_Text_FaeGiftSootheBell:
	.string "Oh! I want you to have this.\p"
	.string "Grandma made it from an old chime\n"
	.string "at the top of MT. PYRE.\p"
	.string "It helps Pokémon feel close to\n"
	.string "whoever is carrying it!$"
