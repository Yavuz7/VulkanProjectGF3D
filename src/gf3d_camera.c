#include "gfc_matrix.h"
#include "gf3d_camera.h"
#include <string.h>

static Camera gf3d_camera = { 0 };
//Matrix4 gf3d_camera = {0};

void gf3d_camera_get_view(Matrix4 *view)
{
    if (!view)return;
    memcpy(view,gf3d_camera.cameraMat,sizeof(Matrix4));
}

void gf3d_camera_set_view(Matrix4 *view)
{
    if (!view)return;
    memcpy(gf3d_camera.cameraMat,view,sizeof(Matrix4));
}

void gf3d_camera_look_at(
    Vector3D position,
    Vector3D target,
    Vector3D up
)
{
    gfc_matrix_view(
        gf3d_camera.cameraMat,
        position,
        target,
        up
    );
}

void gf3d_camera_update_view()
{
	gfc_matrix_identity(gf3d_camera.cameraMat);
	gfc_matrix_scale(gf3d_camera.cameraMat, gf3d_camera.scale);

	gfc_matrix_translate(gf3d_camera.cameraMat, gf3d_camera.scale);

	gfc_matrix_rotate(gf3d_camera.cameraMat, gf3d_camera.cameraMat, gf3d_camera.rotation.x, vector3d(0, 0, 1));
	gfc_matrix_rotate(gf3d_camera.cameraMat, gf3d_camera.cameraMat, gf3d_camera.rotation.y, vector3d(0, 1, 0));
	gfc_matrix_rotate(gf3d_camera.cameraMat, gf3d_camera.cameraMat, gf3d_camera.rotation.z, vector3d(1, 0, 0));

	gfc_matrix_translate(gf3d_camera.cameraMat, gf3d_camera.position);


}

void gf3d_camera_set_position(Vector3D position)
{
    gf3d_camera.position.x = -position.x;
    gf3d_camera.position.y = -position.y;
    gf3d_camera.position.z = -position.z;
}

void gf3d_camera_set_rotation(Vector3D rotation)
{
	gf3d_camera.rotation.x = -rotation.x;
	gf3d_camera.rotation.y = -rotation.y;
	gf3d_camera.rotation.z = -rotation.z;
}

void gf3d_camera_set_scale(Vector3D scale)
{
	gf3d_camera.scale.x = -scale.x;
	gf3d_camera.scale.y = -scale.y;
	gf3d_camera.scale.z = -scale.z;
}

/*eol@eof*/
