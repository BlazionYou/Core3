-- Miniature 0.1 Scale Rancor Boss
rancor_mini_boss = Creature:new {
	objectName = "@mob/creature_names:rancor",
	customName = "Death Jr",
	socialGroup = "rancor",
	faction = "",
	mobType = MOB_CARNIVORE,
	level = 290,
	chanceHit = 0.75,
	damageMin = 1800,
	damageMax = 3200,
	baseXp = 100000,
	baseHAM = 250000,
	baseHAMmax = 300000,
	armor = 2,
	resists = {95,95,-1,95,95,95,-1,-1,-1},
	meatType = "meat_carnivore",
	meatAmount = 2950,
	hideType = "hide_leathery",
	hideAmount = 2877,
	boneType = "bone_mammal",
	boneAmount = 2777,
	milk = 1000,
	tamingChance = 0.05,
	ferocity = 95,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	scale = 0.05,
	templates = {"object/mobile/rancor_hue.iff"},
	hues = { 0, 1, 2, 3, 4, 5, 6, 7 },
	controlDeviceTemplate = "object/intangible/pet/rancor_hue.iff",
	lootGroups = {
		{
			groups = {
				{group = "rancor_boss", chance = 10000000}
			},
			lootChance = 10000000
		}
	},

	primaryWeapon = "unarmed",
	secondaryWeapon = "none",
	conversationTemplate = "",
	primaryAttacks = { {"stunattack",""}, {"intimidationattack",""}, {"creatureareakill",""} },
	secondaryAttacks = { }
}

CreatureTemplates:addCreatureTemplate(rancor_mini_boss, "rancor_mini_boss")
