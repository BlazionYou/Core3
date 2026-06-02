battle_probot = Creature:new {
	objectName = "@droid_name:probe_droid_crafted_advanced",
	socialGroup = "",
	faction = "",
	level = 100,
	mobType = MOB_DROID,
	chanceHit = 20.26,
	damageMin = 1313,
	damageMax = 2468,
	baseXp = 0,
	baseHAM = 42000,
	baseHAMmax = 42069,
	armor = 0,
	resists = {90,90,90,90,90,90,90,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = HERD,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {
		"object/creature/npc/droid/crafted/probe_droid_advanced.iff"
	},
	lootGroups = {},
	defaultAttack = "attack",

	-- Primary and secondary weapon should be different types (rifle/carbine, carbine/pistol, rifle/unarmed, etc)
	-- Unarmed should be put on secondary unless the mobile doesn't use weapons, in which case "unarmed" should be put primary and "none" as secondary
	primaryWeapon = "droid_probot_ranged",
	secondaryWeapon = "unarmed",
	conversationTemplate = "",
}

CreatureTemplates:addCreatureTemplate(battle_probot, "battle_probot")