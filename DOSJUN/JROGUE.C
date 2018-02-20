/* I N C L U D E S /////////////////////////////////////////////////////// */

#include "dosjun.h"

/* D E F I N E S ///////////////////////////////////////////////////////// */

#define HIDE_MULTIPLIER		1.5

/* S T R U C T U R E S /////////////////////////////////////////////////// */

/* G L O B A L S ///////////////////////////////////////////////////////// */

/* F U N C T I O N S ///////////////////////////////////////////////////// */

bool Check_Hide(combatant *source)
{
	if (Has_Buff(source, HIDE_BUFF_NAME))
		return false;

	return Has_Skill(source, skHide);
}

noexport void Hidden_Expires(combatant *source, int argument)
{
	if (gState == gsCombat) {
		Combat_Message("%s is revealed!", source->name);
	}
}

void Hide(combatant *source, combatant *target)
{
	int dexterity = Get_Stat(source, sDexterity);

	if (dexterity > 20) dexterity = 20;
	if (dexterity < 3) dexterity = 3;
	Add_Buff(target, HIDE_BUFF_NAME, exTurnEndChance, 101 - (dexterity * 5), Hidden_Expires, 0);
	Combat_Message("%s is hidden from view.", target->name);
}

bool Check_SneakAttack(combatant *source)
{
	return Has_Buff(source, HIDE_BUFF_NAME);
}

void SneakAttack(combatant *source, combatant *target)
{
	char *source_name = source->name,
		*target_name = target->name;
	item *weapon = Get_Weapon(source);
	stat_value base = Get_Stat(source, Get_Weapon_Stat(weapon));
	stat_value min, max;
	int roll, potency;

	if (Is_Dead(target)) {
		Combat_Message("%s missed their chance.", source_name);
		return;
	}

	base += Get_Stat(source, sHitBonus);
	/* note: dodge bonus is ignored on enemy! */

	if (randint(1, 20) <= base) {
		Combat_Message("%s strikes %s from the shadows!", source_name, target_name);

		Get_Weapon_Damage(source, weapon, &min, &max);
		roll = randint(min, max) * HIDE_MULTIPLIER - Get_Stat(target, sArmour);
		if (roll > 0) {
			Damage(target, roll);

			if (Is_Dead(target)) {
				return;
			}

			if (Has_Skill(source, skBludgeon)) {
				Try_Stun(source, target, sStrength);
			}

			if (Has_Skill(source, skVenom)) {
				potency = source->stats[sIntelligence] / 3;
				potency = potency < 1 ? 1 : potency;
				Try_Poison(source, target, sDexterity, potency);
			}
		} else {
			Combat_Message("The blow glances off.");
		}
	} else {
		Combat_Message("%s sneakily attacks %s, but misses.", source_name, target_name);
	}

	/* TODO: should lose hidden sometimes... */
}