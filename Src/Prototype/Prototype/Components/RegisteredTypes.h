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
