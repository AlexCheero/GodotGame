#pragma once

#include "AttackComponents.h"
#include "AIComponents/FSMStateComponents.h"
#include "AIComponents/NavigationComponents.h"
#include "AIComponents/PatrolComponents.h"

#include "SimpleComponents.h"

//registered types

#define ECS_EVENTS \
    MeleeAttackEvent
#define REGISTERED_COMPONENTS \
    MeleePileInComponent, MeleeWeaponComponent, RangedAttackComponent, ThrowableAttackComponent, \
    GrenadeAttackComponent, GrenadeComponent, JumpSpeedComponent, SpeedComponent, \
    GravityComponent, HealthComponent, DecisionMakingComponent, NavMarginComponent, \
    PatrolmanComponent
#define REGISTERED_TAGS \
    PlayerTag, BotTag, MainCameraTag

//meta types declaration

COMPONENT_META(MeleePileInComponent, "maxPileInDistance");
COMPONENT_META(MeleeWeaponComponent, "hitsConfigName");
COMPONENT_META(RangedAttackComponent, "ammoCount", "distance", "damage", "attackTime");
COMPONENT_META(ThrowableAttackComponent, "ammoCount", "throwableScene", "force", "attackTime");
COMPONENT_META(GrenadeAttackComponent, "ammoCount", "grenadeScene", "force", "attackTime");
COMPONENT_META(GrenadeComponent, "explosionTime", "explosionRadius", "damage");
COMPONENT_META(JumpSpeedComponent, "speed");
COMPONENT_META(SpeedComponent, "speed", "dashSpeed");
COMPONENT_META(GravityComponent, "accelerationUp", "accelerationDown");
COMPONENT_META(HealthComponent, "hp", "maxHp");
COMPONENT_META(DecisionMakingComponent, "criticalHp", "targetPosRecalcDistance");
COMPONENT_META(NavMarginComponent, "margin");
COMPONENT_META(PatrolmanComponent, "viewAngleSmall", "viewAngleBig", "longViewDistance", "shortViewDistance", "looseTargetTime", "lookAroundTime");

TAG_META(PlayerTag);
TAG_META(BotTag);
TAG_META(MainCameraTag);
