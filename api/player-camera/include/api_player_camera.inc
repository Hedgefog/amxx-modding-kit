#if defined _api_player_camera_included
  #endinput
#endif
#define _api_player_camera_included

#include <api_player_camera_const>

#pragma reqlib api_player_camera

/**
 * Activates the player's camera.
 *
 * @param pPlayer The player entity.
 *
 * @noreturn
 */
native PlayerCamera_Activate(const pPlayer);

/**
 * Deactivates the player's camera.
 *
 * @param pPlayer The player entity.
 *
 * @noreturn
 */
native PlayerCamera_Deactivate(const pPlayer);

/**
 * Updates the player's camera.
 *
 * @param pPlayer The player entity.
 *
 * @noreturn
 */
native PlayerCamera_Update(const pPlayer);

/**
 * Checks if the player's camera is active.
 *
 * @param pPlayer The player entity.
 *
 * @return True if the camera is active, false otherwise.
 */
native bool:PlayerCamera_IsActive(const pPlayer);

/**
 * Sets the offset of the player's camera.
 *
 * @param pPlayer     The player entity.
 * @param vecOffset   The offset vector.
 *
 * @noreturn
 */
native PlayerCamera_SetOffset(const pPlayer, const Float:vecOffset[3]);

/**
 * Sets the angles of the player's camera.
 *
 * @param pPlayer     The player entity.
 * @param vecAngles   The angles vector.
 *
 * @noreturn
 */
native PlayerCamera_SetAngles(const pPlayer, const Float:vecAngles[3]);

/**
 * Sets the distance of the player's camera.
 *
 * @param pPlayer    The player entity.
 * @param flDistance The distance value.
 *
 * @noreturn
 */
native PlayerCamera_SetDistance(const pPlayer, Float:flDistance);

/**
 * Sets the axis lock of the player's camera.
 *
 * @param pPlayer     The player entity.
 * @param bLockPitch  Lock Pitch axis.
 * @param bLockYaw    Lock Yaw axis.
 * @param bLockRoll   Lock Roll axis.
 *
 * @noreturn
 */
native PlayerCamera_SetAxisLock(const pPlayer, bool:bLockPitch, bool:bLockYaw, bool:bLockRoll);

/**
 * Gets the target entity of the player's camera.
 *
 * @param pPlayer   The player entity.
 *
 * @return The target entity of the player's camera.
 */
native PlayerCamera_GetTargetEntity(const pPlayer);

/**
 * Sets the target entity of the player's camera.
 *
 * @param pPlayer    The player entity.
 * @param pTarget    The target entity.
 *
 * @noreturn
 */
native PlayerCamera_SetTargetEntity(const pPlayer, const pTarget);

/**
 * Sets the thinking delay of the player's camera.
 *
 * @param pPlayer      The player entity.
 * @param flThinkDelay The thinking delay value.
 *
 * @noreturn
 */
native PlayerCamera_SetThinkDelay(const pPlayer, Float:flThinkDelay);

/**
 * Sets the damping (smoothness) of the player's camera.
 *
 * @param pPlayer     The player entity.
 * @param flDamping   The damping value (0.0 - 1.0).
 *                    - 0.0: no movement
 *                    - 0.5: medium damping (smooth movement)
 *                    - 1.0: no damping (instant movement)
 *
 * @noreturn
 */
native PlayerCamera_SetDamping(const pPlayer, Float:flDamping);

/**
 * Gets the origin of the player's camera.
 *
 * @param pPlayer   The player entity.
 * @param vecOut The vector to store the origin.
 *
 * @return The origin vector of the player's camera.
 */
native PlayerCamera_GetOrigin(const pPlayer, Float:vecOut[]);

/**
 * Sets the origin of the player's camera.
 *
 * @param pPlayer   The player entity.
 * @param vecOrigin The origin vector.
 *
 * @noreturn
 */
native PlayerCamera_SetOrigin(const pPlayer, const Float:vecOrigin[]);

/**
 * Resets the origin of the player's camera.
 *
 * @param pPlayer   The player entity.
 *
 * @noreturn
 */
native PlayerCamera_ResetOrigin(const pPlayer);

/**
 * Function called when the player's camera is activated.
 *
 * @param pPlayer The player entity.
 *
 * @noreturn
 */
forward PlayerCamera_OnActivate(const pPlayer);

/**
 * Function called when the player's camera is activated.
 *
 * @param pPlayer The player entity.
 *
 * @noreturn
 */
forward PlayerCamera_OnActivated(const pPlayer);

/**
 * Function called when the player's camera is deactivated.
 *
 * @param pPlayer The player entity.
 *
 * @noreturn
 */
forward PlayerCamera_OnDeactivate(const pPlayer);

/**
 * Function called when the player's camera is deactivated.
 *
 * @param pPlayer The player entity.
 *
 * @noreturn
 */
forward PlayerCamera_OnDeactivated(const pPlayer);
