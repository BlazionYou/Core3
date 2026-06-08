
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
   ingredientTitleNames = {"frame_assembly", "previous_gen_saber", "activator_coil", "handgrip", "crystal_array", "power_field_insulator", "energizers"},
   -- SLOTS: 0=resource, 1=component (IdenticalSlot → ComponentSlot)
   -- Slot 1: previous-gen saber (type 1 ComponentSlot accepts WeaponObject)
   -- Slot 4: force crystal (type 1 ComponentSlot, applyComponentStats handles it)
   ingredientSlotType = {0, 1, 0, 0, 1, 0, 0},
   resourceTypes = {"mineral", "object/weapon/melee/sword/kinetic_saber/shared_kinetic_saber_training.iff", "metal", "chemical", "object/tangible/component/weapon/lightsaber/shared_lightsaber_module_force_crystal.iff", "gas", "metal"},
   resourceQuantities = {15, 1, 14, 17, 1, 20, 14},
   contribution = {100, 100, 100, 100, 100, 100, 100},


   targetTemplate = "object/weapon/melee/sword/kinetic_saber/shared_kinetic_saber_gen1.iff",

   additionalTemplates = {}

}
ObjectTemplates:addTemplate(object_draft_schematic_weapon_kinetic_saber_kinetic_saber_gen1, "object/draft_schematic/weapon/kinetic_saber/kinetic_saber_gen1.iff")
