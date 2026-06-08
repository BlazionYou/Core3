
object_draft_schematic_weapon_kinetic_saber_kinetic_saber_gen1 = object_draft_schematic_weapon_kinetic_saber_shared_kinetic_saber_gen1:new {

   templateType = DRAFTSCHEMATIC,

   customObjectName = "First Generation Kinetic Saber",

   craftingToolTab = 1, -- weaponsmith tab
   complexity = 16, 
   size = 1, 
   factoryCrateType = "object/factory/factory_crate_weapon.iff",
   
   xpType = "crafting_weapons_general", 
   xp = 400, 

   assemblySkill = "weapon_assembly", 
   experimentingSkill = "weapon_experimentation", 
   customizationSkill = "weapon_customization", 
   factoryCrateSize = 0,   

   customizationOptions = {},
   customizationStringNames = {},
   customizationDefaults = {},

   ingredientTemplateNames = {"craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n"},
   -- IFF slot order (must match shared_kinetic_saber_genN.iff):
   --   0:emitter_shroud 1:primary_crystal 2:activator 3:handgrip
   --   4:focusing_crystals 5:previous_gen_saber 6:energizers
   -- Type 1 = ComponentSlot (accepts the previous-gen weapon at index 5)
   ingredientTitleNames = {"frame_assembly", "primary_crystal", "activator", "handgrip", "focusing_crystals", "previous_gen_saber", "energizers"},
   ingredientSlotType = {0, 0, 0, 0, 0, 1, 0},
   resourceTypes = {"mineral", "mineral", "metal", "chemical", "object/tangible/component/weapon/lightsaber/shared_lightsaber_module_force_crystal.iff", "object/weapon/melee/sword/kinetic_saber/shared_kinetic_saber_training.iff", "metal"},
   resourceQuantities = {15, 1, 14, 17, 1, 1, 14},
   contribution = {100, 100, 100, 100, 100, 100, 100},


   targetTemplate = "object/weapon/melee/sword/kinetic_saber/shared_kinetic_saber_gen1.iff",

   additionalTemplates = {}

}
ObjectTemplates:addTemplate(object_draft_schematic_weapon_kinetic_saber_kinetic_saber_gen1, "object/draft_schematic/weapon/kinetic_saber/kinetic_saber_gen1.iff")
