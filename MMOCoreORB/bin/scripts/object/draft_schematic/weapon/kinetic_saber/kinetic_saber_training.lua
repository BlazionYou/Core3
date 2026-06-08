--Copyright (C) 2010 <SWGEmu>

--This File is part of Core3.

--This program is free software; you can redistribute 
--it and/or modify it under the terms of the GNU Lesser 
--General Public License as published by the Free Software
--Foundation; either version 2 of the License, 
--or (at your option) any later version.

--This program is distributed in the hope that it will be useful, 
--but WITHOUT ANY WARRANTY; without even the implied warranty of 
--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
--See the GNU Lesser General Public License for
--more details.

--You should have received a copy of the GNU Lesser General 
--Public License along with this program; if not, write to
--the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

--Linking Engine3 statically or dynamically with other modules 
--is making a combined work based on Engine3. 
--Thus, the terms and conditions of the GNU Lesser General Public License 
--cover the whole combination.

--In addition, as a special exception, the copyright holders of Engine3 
--give you permission to combine Engine3 program with free software 
--programs or libraries that are released under the GNU LGPL and with 
--code included in the standard release of Core3 under the GNU LGPL 
--license (or modified versions of such code, with unchanged license). 
--You may copy and distribute such a system following the terms of the 
--GNU LGPL for Engine3 and the licenses of the other code concerned, 
--provided that you include the source code of that other code when 
--and as the GNU LGPL requires distribution of source code.

--Note that people who make modified versions of Engine3 are not obligated 
--to grant this special exception for their modified versions; 
--it is their choice whether to do so. The GNU Lesser General Public License 
--gives permission to release a modified version without this exception; 
--this exception also makes it possible to release a modified version


object_draft_schematic_weapon_kinetic_saber_kinetic_saber_training = object_draft_schematic_weapon_kinetic_saber_shared_kinetic_saber_training:new {

   templateType = DRAFTSCHEMATIC,

   customObjectName = "A Training Kinetic Saber",

   craftingToolTab = 1, -- weaponsmith tab
   complexity = 16, 
   size = 1, 
   factoryCrateType = "object/factory/factory_crate_weapon.iff",
   
   xpType = "crafting_weapons_general", 
   xp = 200, 

   assemblySkill = "weapon_assembly", 
   experimentingSkill = "weapon_experimentation", 
   customizationSkill = "weapon_customization", 
   factoryCrateSize = 0,   

   customizationOptions = {},
   customizationStringNames = {},
   customizationDefaults = {},

   ingredientTemplateNames = {"craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n"},
   ingredientTitleNames = {"frame_assembly", "crystal_array", "activator_coil", "handgrip", "focusing_crystals", "power_field_insulator", "energizers"},
   -- SLOTS: 0=resource, 1=component (IdenticalSlot → ComponentSlot), 3=generic component
   ingredientSlotType = {0, 1, 0, 0, 1, 0, 0},
   -- Slot 1: force crystal, Slot 4: krayt pearl (or color crystal)
   resourceTypes = {"mineral", "object/tangible/component/weapon/lightsaber/shared_lightsaber_module_force_crystal.iff", "metal", "chemical", "object/tangible/component/weapon/lightsaber/shared_lightsaber_module_krayt_dragon_pearl.iff", "gas", "metal"},
   resourceQuantities = {10, 1, 10, 12, 1, 10, 10},
   contribution = {100, 100, 100, 100, 100, 100, 100},


   targetTemplate = "object/weapon/melee/sword/kinetic_saber/shared_kinetic_saber_training.iff",

   additionalTemplates = {}

}
ObjectTemplates:addTemplate(object_draft_schematic_weapon_kinetic_saber_kinetic_saber_training, "object/draft_schematic/weapon/kinetic_saber/kinetic_saber_training.iff")
