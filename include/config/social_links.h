#ifndef GUARD_CONFIG_SOCIAL_LINKS_H
#define GUARD_CONFIG_SOCIAL_LINKS_H

#define SOCIAL_LINKS_ENABLED                TRUE    // If FALSE, social links never gain points and give no shiny bonus.

// Shiny bonus
// For every social link at SOCIAL_LINK_MAX_RANK whose type matches one of a newly generated Pokémon's types,
// the game rolls this many additional times for that Pokémon to be shiny (same mechanism as the Shiny Charm).
// Only wild and gift Pokémon whose OT is the player are affected.
#define SOCIAL_LINK_MAXED_SHINY_ROLLS       3

// Pokémon link points, awarded at the end of every battle the player wins or catches a Pokémon in.
// Only Pokémon that were sent out during the battle earn points.
#define SOCIAL_LINK_POINTS_BATTLE           1       // For being sent out in a won battle.
#define SOCIAL_LINK_POINTS_NO_FAINT         2       // Bonus for being sent out and never fainting in that battle.
#define SOCIAL_LINK_POINTS_TRAINER_BONUS    1       // Bonus on top of the above for trainer battles.
#define SOCIAL_LINK_POINTS_LEVEL_UP         3       // For each level gained during a battle. Also awarded in lost battles.

#endif // GUARD_CONFIG_SOCIAL_LINKS_H
