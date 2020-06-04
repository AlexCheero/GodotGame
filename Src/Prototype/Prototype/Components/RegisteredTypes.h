#pragma once

#include "AttackComponents.h"
#include "AIComponents/FSMStateComponents.h"
#include "AIComponents/NavigationComponents.h"
#include "AIComponents/PatrolComponents.h"

#include "SimpleComponents.h"

//registered types

#define REGISTERED_COMPONENTS \
    MeleePileInComponent, MeleeWeaponComponent, RangedAttackComponent, ThrowableAttackComponent, \
    GrenadeAttackComponent, GrenadeComponent, JumpSpeedComponent, SpeedComponent, \
    GravityComponent, HealthComponent, DecisionMakingComponent, NavMarginComponent, \
    PatrolmanComponent
#define REGISTERED_TAGS \
    PlayerTag, BotTag, MainCameraTag

//meta types declaration

COMPONENTS_META(MeleePileInComponent, "maxPileInDistance");
COMPONENTS_META(MeleeWeaponComponent, "hitsConfigName");
COMPONENTS_META(RangedAttackComponent, "ammoCount", "distance", "damage", "attackTime");
COMPONENTS_META(ThrowableAttackComponent, "ammoCount", "throwableScene", "force", "attackTime");
COMPONENTS_META(GrenadeAttackComponent, "ammoCount", "grenadeScene", "force", "attackTime");
COMPONENTS_META(GrenadeComponent, "explosionTime", "explosionRadius", "damage");
COMPONENTS_META(JumpSpeedComponent, "speed");
COMPONENTS_META(SpeedComponent, "speed", "dashSpeed");
COMPONENTS_META(GravityComponent, "accelerationUp", "accelerationDown");
COMPONENTS_META(HealthComponent, "hp", "maxHp");
COMPONENTS_META(DecisionMakingComponent, "criticalHp", "targetPosRecalcDistance");
COMPONENTS_META(NavMarginComponent, "margin");
COMPONENTS_META(PatrolmanComponent, "viewAngleSmall", "viewAngleBig", "longViewDistance", "shortViewDistance", "looseTargetTime", "lookAroundTime");
