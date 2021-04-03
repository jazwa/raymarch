#ifndef OBJECT3D_H
#define OBJECT3D_H

/* Represents any sort of object inhabiting 3d space. Any sort of 
   linear transformations can be applied to these objects: rotations, 
   scaling, and translations. 

   NOT THREAD SAFE! Right now there's no need for it, but may implement 
   in the future. 

*/

#include <Eigen/Core>

using Eigen::Matrix4f, Eigen::Vector3f;

class Object3D {
   private:
      // Right now, all transformations are represented using 
      // 3x3 matrices, these matrices encode translation and rotation
      // TODO: implement quaternions and scaling
      Matrix4f current_transform;
      Matrix4f current_inverse;

      void update_inverse();
      unsigned int update_ref = 0;

   public:
      Object3D() {
         current_transform = Matrix4f::Identity();
      }

      Matrix4f& get_transform_matrix() {
         return current_transform;
      }

      Matrix4f& get_inverse_trans() {
         if (update_ref > 0) {
            update_inverse();
         }
         return current_inverse;
      };

      // Apply various transformation to the object, saves the 
      // final state of the object. These are STATEFUL operations
      void apply_scale(float factor);

      // Rotate based on basic rotations - radians
      // The coordinate system is right-handed, with z pointing up
      void apply_rotate(float rx, float ry, float rz);
      void apply_rotate_x(float rx);
      void apply_rotate_y(float ry);
      void apply_rotate_z(float rz);

      // Move the object
      void apply_translate(Vector3f dir, float mag);
      void apply_translate(Vector3f disp);

      // Apply general 4d transform
      void apply_transform(Matrix4f& trans);

      void reset_transform();

      // Linear interpolate translation.
      // TODO: implement rotational lerp using quaternions
      // TODO: not the really happy with the interface
      Object3D& lerp(Vector3f dest, float t);

};

#endif