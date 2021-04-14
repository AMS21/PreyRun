cmake_minimum_required(VERSION 3.16)

# ##############################################################################
# idLib
# ##############################################################################
set(IDLIB_NAME "idLib")

# Files in the root directory
file(
  GLOB
  IDLIB_GLOBAL_FILES
  "idLib/Base64.cpp"
  "idLib/Base64.h"
  "idLib/BitMsg.cpp"
  "idLib/BitMsg.h"
  "idLib/CmdArgs.cpp"
  "idLib/CmdArgs.h"
  "idLib/Dict.cpp"
  "idLib/Dict.h"
  "idLib/Heap.cpp"
  "idLib/Heap.h"
  "idLib/LangDict.cpp"
  "idLib/LangDict.h"
  "idLib/Lexer.cpp"
  "idLib/Lexer.h"
  "idLib/Lib.cpp"
  "idLib/Lib.h"
  "idLib/MapFile.cpp"
  "idLib/MapFile.h"
  "idLib/Parser.cpp"
  "idLib/Parser.h"
  "idLib/precompiled.h"
  "idLib/Str.cpp"
  "idLib/Str.h"
  "idLib/Timer.cpp"
  "idLib/Timer.h"
  "idLib/Token.cpp"
  "idLib/Token.h")
source_group("idLib" FILES ${IDLIB_GLOBAL_FILES})

# BC
file(
  GLOB
  IDLIB_BV_FILES
  "idLib/bv/Bounds.cpp"
  "idLib/bv/Bounds.h"
  "idLib/bv/Box.cpp"
  "idLib/bv/Box.h"
  "idLib/bv/Frustum.cpp"
  "idLib/bv/Frustum.h"
  "idLib/bv/Sphere.cpp"
  "idLib/bv/Sphere.h")
source_group("idLib\\BV" FILES ${IDLIB_BV_FILES})

# Containers
file(
  GLOB
  IDLIB_CONTAINERS_FILES
  "idLib/containers/BinSearch.h"
  "idLib/containers/BTree.h"
  "idLib/containers/HashIndex.cpp"
  "idLib/containers/HashIndex.h"
  "idLib/containers/HashTable.h"
  "idLib/containers/Hierarchy.h"
  "idLib/containers/LinkList.h"
  "idLib/containers/List.h"
  "idLib/containers/PlaneSet.h"
  "idLib/containers/PreyStack.h"
  "idLib/containers/Queue.h"
  "idLib/containers/Stack.h"
  "idLib/containers/StaticList.h"
  "idLib/containers/Strlist.h"
  "idLib/containers/StrPool.h"
  "idLib/containers/VectorSet.h")
source_group("idLib\\Containers" FILES ${IDLIB_CONTAINERS_FILES})

# Geometry
file(
  GLOB
  IDLIB_GEOMETRY_FILES
  "idLib/geometry/DrawVert.cpp"
  "idLib/geometry/DrawVert.h"
  "idLib/geometry/JointTransform.cpp"
  "idLib/geometry/JointTransform.h"
  "idLib/geometry/Surface.cpp"
  "idLib/geometry/Surface.h"
  "idLib/geometry/Surface_Patch.cpp"
  "idLib/geometry/Surface_Path.h"
  "idLib/geometry/Surface_Polytope.cpp"
  "idLib/geometry/Surface_Polytope.h"
  "idLib/geometry/Surface_SweptSpline.cpp"
  "idLib/geometry/Surface_SweptSpline.h"
  "idLib/geometry/TraceModel.cpp"
  "idLib/geometry/TraceModel.h"
  "idLib/geometry/Winding.cpp"
  "idLib/geometry/Winding.h"
  "idLib/geometry/Winding2D.cpp"
  "idLib/geometry/Winding2D.h")
source_group("idLib\\Geometry" FILES ${IDLIB_GEOMETRY_FILES})

# Hashing
file(
  GLOB
  IDLIB_HASHING_FILES
  "idLib/hashing/CRC8.cpp"
  "idLib/hashing/CRC8.h"
  "idLib/hashing/CRC16.cpp"
  "idLib/hashing/CRC16.h"
  "idLib/hashing/CRC32.cpp"
  "idLib/hashing/CRC32.h"
  "idLib/hashing/Honeyman.cpp"
  "idLib/hashing/Honeyman.h"
  "idLib/hashing/MD4.cpp"
  "idLib/hashing/MD4.h"
  "idLib/hashing/MD5.cpp"
  "idLib/hashing/MD5.h")
source_group("idLib\\Hashing" FILES ${IDLIB_HASHING_FILES})

# Math
file(
  GLOB
  IDLIB_MATH_FILES
  "idLib/math/Angles.cpp"
  "idLib/math/Angles.h"
  "idLib/math/Complex.cpp"
  "idLib/math/Complex.h"
  "idLib/math/Curve.h"
  "idLib/math/Extrapolate.h"
  "idLib/math/Interpolate.h"
  "idLib/math/Lcp.cpp"
  "idLib/math/Lcp.h"
  "idLib/math/Math.cpp"
  "idLib/math/Math.h"
  "idLib/math/Matrix.cpp"
  "idLib/math/Matrix.h"
  "idLib/math/Ode.cpp"
  "idLib/math/Ode.h"
  "idLib/math/Plane.cpp"
  "idLib/math/Plane.h"
  "idLib/math/Pluecker.cpp"
  "idLib/math/Pluecker.h"
  "idLib/math/Polynomial.cpp"
  "idLib/math/Polynomial.h"
  "idLib/math/prey_interpolate.h"
  "idLib/math/prey_math.cpp"
  "idLib/math/prey_math.h"
  "idLib/math/Quat.cpp"
  "idLib/math/Quat.h"
  "idLib/math/Random.h"
  "idLib/math/Rotation.cpp"
  "idLib/math/Ratation.h"
  "idLib/math/Simd.cpp"
  "idLib/math/Simd.h"
  "idLib/math/Simd_3DNow.cpp"
  "idLib/math/Simd_3dNow.h"
  # "idLib/math/Simd_AltiVec.cpp" "idLib/math/Simd_AltiVec.h"
  "idLib/math/Simd_Generic.cpp"
  "idLib/math/Simd_Generic.h"
  "idLib/math/Simd_MMX.cpp"
  "idLib/math/Simd_MMX.h"
  "idLib/math/Simd_SSE.cpp"
  "idLib/math/Simd_SSE.h"
  "idLib/math/Simd_SSE2.cpp"
  "idLib/math/Simd_SSE2.h"
  "idLib/math/Simd_SSE3.cpp"
  "idLib/math/Simd_SSE3.h"
  "idLib/math/Vector.cpp"
  "idLib/math/Vector.h")
source_group("idLib\\Math" FILES ${IDLIB_MATH_FILES})

set(IDLIB_SOURCE_FILES
    ${IDLIB_GLOBAL_FILES} ${IDLIB_BV_FILES} ${IDLIB_CONTAINERS_FILES}
    ${IDLIB_GEOMETRY_FILES} ${IDLIB_HASHING_FILES} ${IDLIB_MATH_FILES})

add_library(${IDLIB_NAME} STATIC ${IDLIB_SOURCE_FILES})

target_include_directories(${IDLIB_NAME} PUBLIC "idLib")
target_precompile_headers(${IDLIB_NAME} PUBLIC "idLib/precompiled.h")
target_compile_definitions(${IDLIB_NAME} PRIVATE _D3SDK __DOOM__ __IDLIB__
                                                 HUMANHEAD _DOTNET_2005)
target_link_libraries(${IDLIB_NAME} PUBLIC project_options)

if(MSVC)
  target_compile_definitions(${IDLIB_NAME} PUBLIC "/DEF ./game/game.def")
endif()

# ##############################################################################
# gamex86.dll #
# ##############################################################################

set(GAME_NAME "gamex86")

# Includes
file(
  GLOB
  GAME_GLOBAL_FILES
  "game/Actor.cpp"
  "game/Actor.h"
  "game/AF.cpp"
  "game/AF.h"
  "game/AFEntity.cpp"
  "game/AFEntity.h"
  "game/BrittleFracture.cpp"
  "game/BrittleFracture.h"
  "game/Camera.cpp"
  "game/Camera.h"
  "game/Entity.cpp"
  "game/Entity.h"
  "game/EntityAdditions.cpp"
  "game/Fx.cpp"
  "game/Fx.h"
  "game/Game.h"
  "game/GameEdit.cpp"
  "game/GameEdit.h"
  "game/Game_local.cpp"
  "game/Game_local.h"
  "game/Game_network.cpp"
  "game/IK.cpp"
  "game/IK.h"
  "game/Item.cpp"
  "game/Item.h"
  "game/Light.cpp"
  "game/Light.h"
  "game/Misc.cpp"
  "game/Misc.h"
  "game/Moveable.cpp"
  "game/Moveable.h"
  "game/Mover.cpp"
  "game/Mover.h"
  "game/MultiplayerGame.cpp"
  "game/MultiplayerGame.h"
  "game/Player.cpp"
  "game/Player.h"
  "game/PlayerIcon.cpp"
  "game/PlayerIcon.h"
  "game/PlayerView.cpp"
  "game/PlayerView.h"
  "game/Projectile.cpp"
  "game/Projectile.h"
  "game/Pvs.cpp"
  "game/Pvs.h"
  "game/SecurityCamera.cpp"
  "game/SecurityCamera.h"
  "game/SmokeParticles.cpp"
  "game/SmokeParticles.h"
  "game/Sound.cpp"
  "game/Sound.h"
  "game/Target.cpp"
  "game/Target.h"
  "game/Trigger.cpp"
  "game/Trigger.h"
  "game/Weapon.cpp"
  "game/Weapon.h"
  "game/WorldSpawn.cpp"
  "game/WorldSpawn.h")
source_group("Game" FILES ${GAME_GLOBAL_FILES})

file(
  GLOB
  GAME_AI_FILES
  "game/ai/AAS.cpp"
  "game/ai/AAS.h"
  "game/ai/AAS_debug.cpp"
  "game/ai/AAS_local.h"
  "game/ai/AAS_pathing.cpp"
  "game/ai/AAS_routing.cpp"
  "game/ai/AI.cpp"
  "game/ai/AI.h"
  "game/ai/AI_events.cpp"
  "game/ai/AI_pathing.cpp")
source_group("Game\\ai" FILES ${GAME_AI_FILES})

file(
  GLOB
  GAME_ANIM_FILES
  "game/anim/Anim.cpp"
  "game/anim/Anim.h"
  "game/anim/Anim_Blend.cpp"
  "game/anim/Anim_Import.cpp"
  "game/anim/Anim_Testmodel.cpp"
  "game/anim/Anim_Testmodel.h")
source_group("Game\\anim" FILES ${GAME_ANIM_FILES})

file(
  GLOB
  GAME_GAMESYS_FILES
  "game/gamesys/Class.cpp"
  "game/gamesys/Class.h"
  "game/gamesys/DebugGraph.cpp"
  "game/gamesys/DebugGraph.h"
  "game/gamesys/Event.cpp"
  "game/gamesys/Event.h"
  "game/gamesys/NoGameTypeInfo.h"
  "game/gamesys/SaveGame.cpp"
  "game/gamesys/SaveGame.h"
  "game/gamesys/SysCmds.cpp"
  "game/gamesys/SysCmds.h"
  "game/gamesys/SysCvar.cpp"
  "game/gamesys/SysCvar.h"
  "game/gamesys/TypeInfo.cpp"
  "game/gamesys/TypeInfo.h")
source_group("Game\\gamesys" FILES ${GAME_GAMESYS_FILES})

file(
  GLOB
  GAME_PHYSICS_FILES
  "game/physics/Clip.cpp"
  "game/physics/Clip.h"
  "game/physics/Force.cpp"
  "game/physics/Force.h"
  "game/physics/Force_Constant.cpp"
  "game/physics/Force_Constant.h"
  "game/physics/Force_Drag.cpp"
  "game/physics/Force_Drag.h"
  "game/physics/Force_Field.cpp"
  "game/physics/Force_Field.h"
  "game/physics/Force_Spring.cpp"
  "game/physics/Force_Spring.h"
  "game/physics/Physics.cpp"
  "game/physics/Physics.h"
  "game/physics/Physics_Actor.cpp"
  "game/physics/Physics_Actor.h"
  "game/physics/Physics_AF.cpp"
  "game/physics/Physics_AF.h"
  "game/physics/Physics_Base.cpp"
  "game/physics/Physics_Base.h"
  "game/physics/Physics_Monster.cpp"
  "game/physics/Physics_Monster.h"
  "game/physics/Physics_Parametric.cpp"
  "game/physics/Physics_Parametric.h"
  "game/physics/Physics_Player.cpp"
  "game/physics/Physics_Player.h"
  "game/physics/Physics_PreyPlayer.cpp"
  "game/physics/Physics_PreyPlayer.h"
  "game/physics/Physics_RigidBody.cpp"
  "game/physics/Physics_RigidBody.h"
  "game/physics/Physics_Static.cpp"
  "game/physics/Physics_Static.h"
  "game/physics/Physics_StaticMulti.cpp"
  "game/physics/Physics_StaticMulti.h"
  "game/physics/Push.cpp"
  "game/physics/Push.h")
source_group("Game\\physics" FILES ${GAME_PHYSICS_FILES})

file(
  GLOB
  GAME_SCRIPT_FILES
  "game/script/Script_Compiler.cpp"
  "game/script/Script_Compiler.h"
  "game/script/Script_Interpreter.cpp"
  "game/script/Script_Interpreter.h"
  "game/script/Script_Program.cpp"
  "game/script/Script_Program.h"
  "game/script/Script_Thread.cpp"
  "game/script/Script_Thread.h")
source_group("Game\\script" FILES ${GAME_SCRIPT_FILES})

file(
  GLOB
  PREY_GLOBAL_FILES
  "prey/anim_baseanim.cpp"
  "prey/anim_baseanim.h"
  "prey/force_converge.cpp"
  "prey/force_converge.h"
  "prey/game_afs.cpp"
  "prey/game_afs.h"
  "prey/game_alarm.cpp"
  "prey/game_alarm.h"
  "prey/game_anim.cpp"
  "prey/game_anim.h"
  "prey/game_animatedentity.cpp"
  "prey/game_animatedentity.h"
  "prey/game_animatedgui.cpp"
  "prey/game_animatedgui.h"
  "prey/game_animator.cpp"
  "prey/game_animator.h"
  "prey/game_animBlend.cpp"
  "prey/game_animBlend.h"
  "prey/game_animDriven.cpp"
  "prey/game_animDriven.h"
  "prey/game_arcadegame.cpp"
  "prey/game_arcadegame.h"
  "prey/game_barrel.cpp"
  "prey/game_barrel.h"
  "prey/game_bindController.cpp"
  "prey/game_bindController.h"
  "prey/game_blackjack.cpp"
  "prey/game_blackjack.h"
  "prey/game_cards.cpp"
  "prey/game_cards.h"
  "prey/game_cilia.cpp"
  "prey/game_cilia.h"
  "prey/game_console.cpp"
  "prey/game_console.h"
  "prey/game_damagetester.cpp"
  "prey/game_damagetester.h"
  "prey/game_dda.cpp"
  "prey/game_dda.h"
  "prey/game_deathwraith.cpp"
  "prey/game_deathwraith.h"
  "prey/game_debrisspawner.cpp"
  "prey/game_debrisspawner.h"
  "prey/game_dock.cpp"
  "prey/game_dock.h"
  "prey/game_dockedgun.cpp"
  "prey/game_dockedgun.h"
  "prey/game_door.cpp"
  "prey/game_door.h"
  "prey/game_eggspawner.cpp"
  "prey/game_eggspawner.h"
  "prey/game_energynode.cpp"
  "prey/game_energynode.h"
  "prey/game_entityfx.cpp"
  "prey/game_entityfx.h"
  "prey/game_entityspawner.cpp"
  "prey/game_entityspawner.h"
  "prey/game_fixedpod.cpp"
  "prey/game_fixedpod.h"
  "prey/game_forcefield.cpp"
  "prey/game_forcefield.h"
  "prey/game_fxinfo.cpp"
  "prey/game_fxinfo.h"
  "prey/game_gibbable.cpp"
  "prey/game_gibbable.h"
  "prey/game_gravityswitch.cpp"
  "prey/game_gravityswitch.h"
  "prey/game_guihand.cpp"
  "prey/game_guihand.h"
  "prey/game_gun.cpp"
  "prey/game_gun.h"
  "prey/game_hand.cpp"
  "prey/game_hand.h"
  "prey/game_handcontrol.cpp"
  "prey/game_handcontrol.h"
  "prey/game_healthbasin.cpp"
  "prey/game_healthbasin.h"
  "prey/game_healthspore.cpp"
  "prey/game_healthspore.h"
  "prey/game_inventory.cpp"
  "prey/game_inventory.h"
  "prey/game_itemautomatic.cpp"
  "prey/game_itemautomatic.h"
  "prey/game_itemcabinet.cpp"
  "prey/game_itemcabinet.h"
  "prey/game_jukebox.cpp"
  "prey/game_jukebox.h"
  "prey/game_jumpzone.cpp"
  "prey/game_jumpzone.h"
  "prey/game_light.cpp"
  "prey/game_light.h"
  "prey/game_lightfixture.cpp"
  "prey/game_lightfixture.h"
  "prey/game_mine.cpp"
  "prey/game_mine.h"
  "prey/game_misc.cpp"
  "prey/game_misc.h"
  "prey/game_modeldoor.cpp"
  "prey/game_modeldoor.h"
  "prey/game_modeltoggle.cpp"
  "prey/game_modeltoggle.h"
  "prey/game_mountedgun.cpp"
  "prey/game_mountedgun.h"
  "prey/game_moveable.cpp"
  "prey/game_moveable.h"
  "prey/game_mover.cpp"
  "prey/game_mover.h"
  "prey/game_note.cpp"
  "prey/game_note.h"
  "prey/game_organtrigger.cpp"
  "prey/game_organtrigger.h"
  "prey/game_player.cpp"
  "prey/game_player.h"
  "prey/game_playerview.cpp"
  "prey/game_playerview.h"
  "prey/game_pod.cpp"
  "prey/game_pod.h"
  "prey/game_podspawner.cpp"
  "prey/game_podspawner.h"
  "prey/game_poker.cpp"
  "prey/game_poker.h"
  "prey/game_portal.cpp"
  "prey/game_portal.h"
  "prey/game_portalframe.cpp"
  "prey/game_portalframe.h"
  "prey/game_proxdoor.cpp"
  "prey/game_proxdoor.h"
  "prey/game_rail.cpp"
  "prey/game_rail.h"
  "prey/game_railshuttle.cpp"
  "prey/game_railshuttle.h"
  "prey/game_renderentity.cpp"
  "prey/game_renderentity.h"
  "prey/game_safeDeathVolume.cpp"
  "prey/game_safeDeathVolume.h"
  "prey/game_securityeye.cpp"
  "prey/game_securityeye.h"
  "prey/game_shuttle.cpp"
  "prey/game_shuttle.h"
  "prey/game_shuttledock.cpp"
  "prey/game_shuttledock.h"
  "prey/game_shuttletransport.cpp"
  "prey/game_shuttletransport.h"
  "prey/game_skybox.cpp"
  "prey/game_skybox.h"
  "prey/game_slots.cpp"
  "prey/game_slots.h"
  "prey/game_sphere.cpp"
  "prey/game_sphere.h"
  "prey/game_spherepart.cpp"
  "prey/game_spherepart.h"
  "prey/game_spring.cpp"
  "prey/game_spring.h"
  "prey/game_sunCorona.cpp"
  "prey/game_sunCorona.h"
  "prey/game_talon.cpp"
  "prey/game_talon.h"
  "prey/game_targetproxy.cpp"
  "prey/game_targetproxy.h"
  "prey/game_targets.cpp"
  "prey/game_targets.h"
  "prey/game_trackmover.cpp"
  "prey/game_trackmover.h"
  "prey/game_trigger.cpp"
  "prey/game_trigger.h"
  "prey/game_tripwire.cpp"
  "prey/game_tripwire.h"
  "prey/game_utils.cpp"
  "prey/game_utils.h"
  "prey/game_vehicle.cpp"
  "prey/game_vehicle.h"
  "prey/game_vomiter.cpp"
  "prey/game_vomiter.h"
  "prey/game_weaponHandState.cpp"
  "prey/game_weaponHandState.h"
  "prey/game_woundmanager.cpp"
  "prey/game_woundmanager.h"
  "prey/game_wraith.cpp"
  "prey/game_wraith.h"
  "prey/game_zone.cpp"
  "prey/game_zone.h"
  "prey/physics_delta.cpp"
  "prey/physics_delta.h"
  "prey/physics_preyai.cpp"
  "prey/physics_preyai.h"
  "prey/physics_preyparametric.cpp"
  "prey/physics_preyparametric.h"
  "prey/physics_simple.cpp"
  "prey/physics_simple.h"
  "prey/physics_vehicle.cpp"
  "prey/physics_vehicle.h"
  "prey/prey_animator.cpp"
  "prey/prey_animator.h"
  "prey/prey_baseweapons.cpp"
  "prey/prey_baseweapons.h"
  "prey/prey_beam.cpp"
  "prey/prey_beam.h"
  "prey/prey_bonecontroller.cpp"
  "prey/prey_bonecontroller.h"
  "prey/prey_camerainterpolator.cpp"
  "prey/prey_camerainterpolator.h"
  "prey/prey_firecontroller.cpp"
  "prey/prey_firecontroller.h"
  "prey/prey_game.cpp"
  "prey/prey_game.h"
  "prey/prey_items.cpp"
  "prey/prey_items.h"
  "prey/prey_liquid.cpp"
  "prey/prey_liquid.h"
  "prey/prey_local.cpp"
  "prey/prey_local.h"
  "prey/prey_projectile.cpp"
  "prey/prey_projectile.h"
  "prey/prey_projectileautocannon.cpp"
  "prey/prey_projectileautocannon.h"
  "prey/prey_projectilebounce.cpp"
  "prey/prey_projectilebounce.h"
  "prey/prey_projectilebug.cpp"
  "prey/prey_projectilebug.h"
  "prey/prey_projectilebugtrigger.cpp"
  "prey/prey_projectilebugtrigger.h"
  "prey/prey_projectilecocoon.cpp"
  "prey/prey_projectilecocoon.h"
  "prey/prey_projectilecrawlergrenade.cpp"
  "prey/prey_projectilecrawlergrenade.h"
  "prey/prey_projectilefreezer.cpp"
  "prey/prey_projectilefreezer.h"
  "prey/prey_projectilegasbagpod.cpp"
  "prey/prey_projectilegasbagpod.h"
  "prey/prey_projectilehiderweapon.cpp"
  "prey/prey_projectilehiderweapon.h"
  "prey/prey_projectilemine.cpp"
  "prey/prey_projectilemine.h"
  "prey/prey_projectilerifle.cpp"
  "prey/prey_projectilerifle.h"
  "prey/prey_projectilerocketlauncher.cpp"
  "prey/prey_projectilerocketlauncher.h"
  "prey/prey_projectileshuttle.cpp"
  "prey/prey_projectileshuttle.h"
  "prey/prey_projectilesoulcannon.cpp"
  "prey/prey_projectilesoulcannon.h"
  "prey/prey_projectilespiritarrow.cpp"
  "prey/prey_projectilespiritarrow.h"
  "prey/prey_projectiletracking.cpp"
  "prey/prey_projectiletracking.h"
  "prey/prey_projectiletrigger.cpp"
  "prey/prey_projectiletrigger.h"
  "prey/prey_projectilewrench.cpp"
  "prey/prey_projectilewrench.h"
  "prey/prey_script_thread.cpp"
  "prey/prey_script_thread.h"
  "prey/prey_sound.cpp"
  "prey/prey_sound.h"
  "prey/prey_soundleadincontroller.cpp"
  "prey/prey_soundleadincontroller.h"
  "prey/prey_spiritbridge.cpp"
  "prey/prey_spiritbridge.h"
  "prey/prey_spiritproxy.cpp"
  "prey/prey_spiritproxy.h"
  "prey/prey_spiritsecret.cpp"
  "prey/prey_spiritsecret.h"
  "prey/prey_vehiclefirecontroller.cpp"
  "prey/prey_vehiclefirecontroller.h"
  "prey/prey_weaponautocannon.cpp"
  "prey/prey_weaponautocannon.h"
  "prey/prey_weaponcrawlergrenade.cpp"
  "prey/prey_weaponcrawlergrenade.h"
  "prey/prey_weaponfirecontroller.cpp"
  "prey/prey_weaponfirecontroller.h"
  "prey/prey_weaponhider.cpp"
  "prey/prey_weaponhider.h"
  "prey/prey_weaponrifle.cpp"
  "prey/prey_weaponrifle.h"
  "prey/prey_weaponrocketlauncher.cpp"
  "prey/prey_weaponrocketlauncher.h"
  "prey/prey_weaponsoulstripper.cpp"
  "prey/prey_weaponsoulstripper.h"
  "prey/prey_weaponspiritbow.cpp"
  "prey/prey_weaponspiritbow.h"
  "prey/sys_debugger.cpp"
  "prey/sys_debugger.h"
  "prey/sys_preycmds.cpp"
  "prey/sys_preycmds.h")

source_group("Prey" FILES ${PREY_GLOBAL_FILES})

file(
  GLOB
  PREY_AI_FILES
  "prey/ai_centurion.cpp"
  "prey/ai_centurion.h"
  "prey/ai_crawler.cpp"
  "prey/ai_crawler.h"
  "prey/ai_creaturex.cpp"
  "prey/ai_creaturex.h"
  "prey/ai_droid.cpp"
  "prey/ai_droid.h"
  "prey/ai_gasbag_simple.cpp"
  "prey/ai_gasbag_simple.h"
  "prey/ai_harvester_simple.cpp"
  "prey/ai_harvester_simple.h"
  "prey/ai_hunter_simple.cpp"
  "prey/ai_hunter_simple.h"
  "prey/ai_inspector.cpp"
  "prey/ai_inspector.h"
  "prey/ai_jetpack_harvester_simple.cpp"
  "prey/ai_jetpack_harvester_simple.h"
  "prey/ai_keeper_simple.cpp"
  "prey/ai_keeper_simple.h"
  "prey/ai_mutate.cpp"
  "prey/ai_mutate.h"
  "prey/ai_mutilatedhuman.cpp"
  "prey/ai_mutilatedhuman.h"
  "prey/ai_passageway.cpp"
  "prey/ai_passageway.h"
  "prey/ai_possessedTommy.cpp"
  "prey/ai_possessedTommy.h"
  "prey/ai_reaction.cpp"
  "prey/ai_reaction.h"
  "prey/ai_spawncase.cpp"
  "prey/ai_spawncase.h"
  "prey/ai_speech.cpp"
  "prey/ai_speech.h"
  "prey/ai_sphereboss.cpp"
  "prey/ai_sphereboss.h"
  "prey/game_monster_ai.cpp"
  "prey/game_monster_ai.h"
  "prey/game_monster_ai_events.cpp")
source_group("Prey\\ai" FILES ${PREY_AI_FILES})

file(
  GLOB
  PREYRUN_FILES
  "PreyRun/AutoCmd.cpp"
  "PreyRun/AutoCmd.hpp"
  "PreyRun/Backup.cpp"
  "PreyRun/Backup.hpp"
  "PreyRun/Constants.hpp"
  "PreyRun/Cvar.cpp"
  "PreyRun/Cvar.hpp"
  "PreyRun/GameTimer.cpp"
  "PreyRun/GameTimer.hpp"
  "PreyRun/Hooking.cpp"
  "PreyRun/Hooking.hpp"
  "PreyRun/Interprocess.cpp"
  "PreyRun/Interprocess.hpp"
  "PreyRun/Logging.cpp"
  "PreyRun/Logging.hpp"
  "PreyRun/Monitor.hpp"
  "PreyRun/PreyRun.hpp"
  "PreyRun/Random.hpp"
  "PreyRun/Thread.hpp"
  "PreyRun/Utility.cpp"
  "PreyRun/Utility.hpp")
source_group("PreyRun" FILES ${PREYRUN_FILES})

file(
  GLOB
  PREYRUN_GHOSTING_FILES
  "PreyRun/Ghosting/GhostEntity.cpp"
  "PreyRun/Ghosting/GhostEntity.hpp"
  "PreyRun/Ghosting/GhostManager.cpp"
  "PreyRun/Ghosting/GhostManager.hpp"
  "PreyRun/Ghosting/GhostRecord.cpp"
  "PreyRun/Ghosting/GhostRecord.hpp")
source_group("PreyRun\\Ghosting" FILES ${PREYRUN_GHOSTING_FILES})

set(GAME_SOURCE_FILES
    ${GAME_GLOBAL_FILES}
    ${GAME_AI_FILES}
    ${GAME_ANIM_FILES}
    ${GAME_GAMESYS_FILES}
    ${GAME_PHYSICS_FILES}
    ${GAME_SCRIPT_FILES}
    ${PREY_GLOBAL_FILES}
    ${PREY_AI_FILES}
    ${PREYRUN_FILES}
    ${PREYRUN_GHOSTING_FILES})

add_library(${GAME_NAME} SHARED ${GAME_SOURCE_FILES})
target_link_libraries(${GAME_NAME} PRIVATE ${IDLIB_NAME})

if(MSVC)
  target_compile_definitions(${GAME_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS
                                                  _SCL_SECURE_NO_WARNINGS)
endif()

# Definitions for gamex86.dll
target_compile_definitions(
  ${GAME_NAME} PRIVATE _D3SDK __DOOM__ GAME_DLL HUMANHEAD _DOTNET_2005
                       _XKEYCHECK_H)
