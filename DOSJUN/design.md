# DOSJUN Design
DOSJUN is a turn-based dungeon crawler.

## Setting
The party turned up to play an Escape the Room game but it turns out to be an actual dungeon. Bizarre-ty insues.

## Party
The party consists of 6 characters.

## Character
A character has these stats:
- Name
- HP / MaxHP
- MP / MaxMP

## Scripting
Zone tiles can have scripts attached them. Scripts are written in a fairly simple syntax but then compiled to a bytecode which the engine will execute. The engine has an accumulator and several general-purpose storage locations.

### Commands
- `Load variable/literal`: put value into accumulator
- `Save variable`: put accumulator value into variable
- `If expression`, `Else`, `EndIf`
- `Return`
- `SetTileDescription x, y, string_id`
- `SetTileColour x, y, surface, colour_id`: surface can be N/E/S/W/C/F
- `Teleport zone, x, y, transition`: transition types - silent, stairs, teleport

### Variables
There are three variable scopes.
- _Globals_: accessible from every zone
- _Locals_: accessible from every script in this zone
- _Temporary_: only exists during this script's lifetime

Globals and Locals need to be declared using `Global x` or `Local x`. Globals need to be declared in the same order every time; to help with this, `Include "SCRIPT.DS"` can be used.