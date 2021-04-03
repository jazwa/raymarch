
#include <object3d.h>
#include <Eigen/LU>

using Eigen::Matrix4f, Eigen::Vector3f;

void Object3D::apply_transform(Matrix4f& mat) {
    this->current_transform = mat * this->current_transform;
    update_ref++;
}

void Object3D::reset_transform() {
    this->current_transform = Matrix4f::Identity();
    update_ref++;
}


void Object3D::update_inverse() {
    // The product of invertible matrices is invertible.
    // This requires Eigen/LU, but to my knowledge, for 4by4
    // matrices, a hardcoded formula for the inverse is used.
    this->current_inverse = (this->current_transform).inverse();
}

void Object3D::apply_rotate(float rx, float ry, float rz) {
    
    const float cosx = cos(rx);
    const float sinx = sin(rx);
    const float cosy = cos(ry);
    const float siny = sin(ry);
    const float cosz = cos(rz);
    const float sinz = sin(rz);

    Matrix4f rotate_mat = Matrix4f();
    rotate_mat << cosz*cosy, cosz*siny*sinx - sinz*cosx, cosz*siny*cosx + sinz*sinx, 0,
                  sinz*cosy, sinz*siny*sinx + cosz*cosx, sinz*siny*cosx - cosz*sinx, 0,
                  (-1)*siny,                  cosy*sinx,                  cosy*cosx, 0,
                          0,                          0,                          0, 1;

    this->apply_transform(rotate_mat);
}

void Object3D::apply_rotate_x(float rx) {
    const float cosx = cos(rx);
    const float sinx = sin(rx);
    Matrix4f rotate_x_mat = Matrix4f();
    rotate_x_mat << 1,    0,         0, 0,
                    0, cosx, (-1)*sinx, 0,
                    0, sinx,      cosx, 0,
                    0,    0,         0, 1;

    this->apply_transform(rotate_x_mat);
}

void Object3D::apply_rotate_y(float ry) {
    const float cosy = cos(ry);
    const float siny = sin(ry);
    Matrix4f rotate_y_mat = Matrix4f();
    rotate_y_mat << cosy,      0, siny, 0,
                    0,         1,    0, 0,
                    (-1)*siny, 0, cosy, 0,
                    0,         0,    0, 1;

    this->apply_transform(rotate_y_mat);
}

void Object3D::apply_rotate_z(float rz) {
    const float cosz = cos(rz);
    const float sinz = sin(rz);
    Matrix4f rotate_z_mat = Matrix4f();
    rotate_z_mat << cosz, (-1)*sinz, 0, 0, 
                    sinz, cosz,      0, 0, 
                    0,    0,         1, 0,
                    0,    0,         0, 1;

    this->apply_transform(rotate_z_mat);
}

void Object3D::apply_translate(Vector3f disp) {
    Matrix4f mat = Matrix4f();
    mat << 1,0,0,disp[0],
           0,1,0,disp[1],
           0,0,1,disp[2],
           0,0,0,      1;

    this->apply_transform(mat);
}

void Object3D::apply_translate(Vector3f dir, float mag) {
    apply_translate(mag*(dir.normalized()));
}
/* 

void Object3D::apply_scale(float f) {
    assert(false); // NOT implemented
}
 */
