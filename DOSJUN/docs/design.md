# DOSJUN Design
DOSJUN is a turn-based dungeon crawler.

## Setting
The party turned up to play an Escape the Room game but it turns out to be an actual dungeon. Bizarre-ty insues. See Setting doc.

## Party
The party consists of 6 characters.

## Character
A character has these stats:
- Name
- Strength
- Dexterity
- Intelligence
- HP / MaxHP
- MP / MaxMP
- Armour
- Toughness
- Min / MaxDamage
- Hit Bonus
- Dodge Bonus

## Scripting
Zone tiles can have scripts attached them. Scripts are written in a fairly simple syntax but then compiled to a bytecode which the engine will execute. The engine is a stack machine with a some partitioned storage.

### Commands
- `SetTileDescription x, y, string_id`
- `SetTileColour x, y, surface, colour_id`: surface can be N/E/S/W/C/F
- `Teleport zone, x, y, facing, transition`: transition types - silent, stairs, teleport

### Variables
There are three variable scopes.
- _Globals_: accessible from every zone
- _Locals_: accessible from every script in this zone
- _Temporary_: only exists during this script's lifetime

Globals and Locals need to be declared using `Global x` or `Local x`. Globals need to be declared in the same order every time; to help with this, `Include "SCRIPT.JC"` can be used.

### Conversations
Conversations work using a pseudo-state machine. A conversation is stated with `Converse npc, state`. States are declared with `State...EndState` instead of `Script...EndScript` but otherwise function similarly. `State`s are not visible in the dungeon editor. Conversation options are declared with `Option state, string`. When the script interpreter exits a `State` function, it checks to see if any options were declared; if so, it waits for the user to pick one, then jumps to the state of the option that was picked. If there were no options, it internally runs `EndConverse`. Either way, it clears all declared options at this point. Calling `Combat` does not automatically end conversations.

The commands `Text`, `PcSpeak`, `PcAction`, `NpcSpeak`, and `NpcAction` use the conversation window in conversation mode, rather than the normal dungeon text box.

## Attitude System
DOSJUN keeps track of Attitude values for PCs and factions. PCs will act differently depending on their Attitude, and many faction tasks will be unavailable if the Attitude value is out of a given range.

| Value | Name | Factions (starting value) |
| ----- | ---- | ------------------------- |
| 200 | Love? | - |
| 100 | Trusting | (all PCs) |
| 50 | Friendly | Colony |
| 0 | Neutral | Penitents |
| -50 | Distrust | Cult |
| -100 | Hatred | Reanimated, Ascended |