#ifndef __GF3D_CAMERA_H__
#define __GF3D_CAMERA_H__

#include "gfc_matrix.h"

typedef struct
{
	Matrix4 cameraMat; //Final matrix to become view matrix
	Vector3D scale;
	Vector3D position;
	Vector3D rotation; //x = pitch, y = roll , z = yaw

}Camera;


/**
 * @brief get the current camera view
 * @param view output, the matrix provided will be populated with the current camera information
 */
void gf3d_camera_get_view(Matrix4 *view);

/**
 * @brief set the current camera based on the matrix provided
 */
void gf3d_camera_set_view(Matrix4 *view);

/**
 * @brief set the camera properties based on position and direction that the camera should be looking
 * @param position the location for the camera
 * @param target the point the camera should be looking at
 * @param up the direction considered to be "up"
 */
void gf3d_camera_look_at(
    Vector3D position,
    Vector3D target,
    Vector3D up
);

/**
 * @brief explicitely set the camera positon, holding all other parameters the same
 * @param position the new position for the camera
 */
void gf3d_camera_set_position(Vector3D position);

/**
* @brief explicitely set the camera rotation, holding all other parameters the same
* @param new rotation pitch x, roll y, yaw z
*/

void gf3d_camera_set_rotation(Vector3D rotation);

/**
* @brief explicitely set the camera scale, holding all other parameters the same
* @param new scale for the camera
*/

void gf3d_camera_set_scale(Vector3D scale);

/**
 * @brief move the camera relatively based on the vector provided
 * @param move the ammount to move the camera
 */
void gf3d_camera_move(Vector3D move);

/**
* @brief Update camera view
*/
void gf3d_camera_update_view();

#endif
