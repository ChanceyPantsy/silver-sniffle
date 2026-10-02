# silver-sniffle

A Pokémon Emerald ROM hack with Pokémon from every generation and a **social link** system.

Based off RHH's pokeemerald-expansion 1.17.1 https://github.com/rh-hideout/pokeemerald-expansion/

The expansion provides all Gen 1–9 species, their learnsets, sprites (front, back, icon, follower and shiny) and modern battle mechanics. See [`FEATURES.md`](FEATURES.md) for everything it includes and [`CREDITS.md`](CREDITS.md) for its contributors.

## Building

Follow [`INSTALL.md`](INSTALL.md). On Debian/Ubuntu the short version is:

```sh
sudo apt install build-essential binutils-arm-none-eabi gcc-arm-none-eabi libnewlib-arm-none-eabi libpng-dev python3
make -j$(nproc)            # builds pokeemerald.gba
make check -j$(nproc)      # runs the test suite
make check TESTS="(Social Links)"   # runs only the social link tests
```

Never commit or share the built `.gba`. Release the hack as a patch (`.bps`/`.ups`) made against a clean Emerald ROM.

## Pulling in newer expansion releases

```sh
git remote add upstream https://github.com/rh-hideout/pokeemerald-expansion.git   # once
git fetch upstream --tags
git merge expansion/<version>
```

## Social links

Social links are relationships that rank up from 0 to 10 (`SOCIAL_LINK_MAX_RANK`). When a link is maxed, every wild or gift Pokémon that shares the link's type gets `SOCIAL_LINK_MAXED_SHINY_ROLLS` extra shiny rolls, the same mechanism as the Shiny Charm. Links of other types don't help, and maxed links of the same type stack.

| File | What's in it |
| --- | --- |
| `include/constants/social_links.h` | Link IDs |
| `src/data/social_links.h` | The roster: name, type and rank caps for each link |
| `include/config/social_links.h` | Point values and the shiny bonus |
| `src/social_links.c` | Implementation |

### Kinds of links

- **Pokémon links** (`SOCIAL_LINK_STARTER`, `SOCIAL_LINK_EVENT_MON_1`–`4`) are tied to one specific Pokémon. Their type is that Pokémon's primary type when it was linked. The starter is linked automatically on Route 101. Link event Pokémon, such as gifts or guaranteed legendaries, from their event scripts. These links grow at the end of every battle:
  - Sent out in a won battle (or one where a Pokémon was caught): `SOCIAL_LINK_POINTS_BATTLE`, +`SOCIAL_LINK_POINTS_TRAINER_BONUS` against trainers.
  - Sent out and never fainted that battle: +`SOCIAL_LINK_POINTS_NO_FAINT`.
  - Each level gained during the battle: +`SOCIAL_LINK_POINTS_LEVEL_UP`.

  Link battles, the Battle Frontier and Trainer Hill don't count.
- **NPC links** (rival, Wally, Steven, gym leaders, Elite Four) only grow when an event script says so. That way each NPC can have its own way of earning points: talking, gifts, battles, side quests.

### Rank caps

A link can't rank up past its current cap; points over the cap are discarded. Caps come from flags, so they follow story progress. By default:
- Pokémon links, the rival, Wally and Steven start at 2 and gain about one rank per badge, reaching 10 after the Hall of Fame.
- Gym leaders cap at 3, then 7 after you beat their gym, then 10 after the Hall of Fame.
- The Elite Four cap at 3 until the Hall of Fame.

To change a cap, edit the gate lists in `src/data/social_links.h`. Any flag works as a gate, so cutscenes and side events can raise caps too.

### Script commands

All arguments accept values or variables.

```
sociallink_addpoints SOCIAL_LINK_ROXANNE, 10       @ VAR_RESULT = TRUE if it ranked up
sociallink_getrank SOCIAL_LINK_ROXANNE             @ VAR_RESULT = rank
sociallink_getrankcap SOCIAL_LINK_ROXANNE          @ VAR_RESULT = current cap
sociallink_setrank SOCIAL_LINK_ROXANNE, 5          @ ignores the cap, for story moments
sociallink_bindpartymon SOCIAL_LINK_EVENT_MON_1, 0 @ link the Pokémon in party slot 0
sociallink_bindspecies SOCIAL_LINK_EVENT_MON_1, SPECIES_GROUDON  @ party first, then PC
sociallink_getpartyslot SOCIAL_LINK_STARTER        @ VAR_RESULT = slot, or PARTY_SIZE
```

The bind commands set `VAR_RESULT` to `SOCIAL_LINK_BIND_OK` or `SOCIAL_LINK_BIND_FAILED`. Binding fails if the link is already bound or the Pokémon already has a link.

For example, an NPC that deepens its link when you talk to it:

```
	sociallink_addpoints SOCIAL_LINK_WALLY, 5
	goto_if_eq VAR_RESULT, TRUE, Wally_EventScript_RankUp
```

Linking a legendary after a scripted encounter:

```
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Groudon_EventScript_NotCaught
	sociallink_bindspecies SOCIAL_LINK_EVENT_MON_1, SPECIES_GROUDON
```

### Fae

Fae is the granddaughter of the Mt. Pyre orb keepers. She's staying in Oldale and wants to see you succeed.

- **Meeting her:** she stands just north of Oldale on Route 103. On your first walk past, two wild Pokémon jump out and you fight them together as a tag battle, with her Chingling beside your team. Her link starts at rank 1 and she gives you 2 Potions.
- **Her link:** a plain friendship link with no type, so maxing it doesn't boost any shinies.
- **Signature Pokémon:** Chingling, which evolves into Chimecho. In Emerald, Chimecho lives only at the summit of Mt. Pyre, where her grandparents keep the orbs.
- **Hanging out:** talk to her afterwards to hang out. Each hang-out plays a scene for her current rank and adds points. When she reaches her rank cap she sends you off to adventure instead, so hang-outs can't be repeated for free points.
- **Gifts:** she gives a small gift the first time her link reaches certain ranks. Rank 2 gives a Soothe Bell.
- **Partner:** her team is `PARTNER_FAE` in `src/data/battle_partners.party`. Use it with `multi_fixed_2_vs_1`, `multi_fixed_2_vs_2` or the wild version seen in her intro.
- **Name:** her name-box name is `SP_NAME_FAE` in `src/data/speaker_names.h`. Her link name is in `src/data/social_links.h`. Her scripts and dialogue are at the end of `data/maps/Route103/scripts.inc`.

Her progress through the story is stored in `VAR_FAE_STATE`. She uses Leaf's sprites as a placeholder; a battle partner needs a trainer picture with a back view, and only a few have one.

### Adding a link

1. Add an ID in `include/constants/social_links.h` and bump `NUM_SOCIAL_LINKS`. Pokémon links must stay at the start of the list (update `NUM_POKEMON_SOCIAL_LINKS`).
2. Add its entry to `gSocialLinksInfo` in `src/data/social_links.h`.

Changing the number of links changes the save layout, so start a new save afterwards.

## Legal

Pokémon and all related names and artwork are property of Nintendo, Game Freak and The Pokémon Company. This is a non-commercial fan project. Don't charge for it or accept money tied to it.
