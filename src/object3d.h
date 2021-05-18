#ifndef OBJECT3D_H
#define OBJECT3D_H

/* Represents any sort of object inhabiting 3d space. Any sort of 
   linear transformations can be applied to these objects: rotations, 
   scaling, and translations. 

   NOT THREAD SAFE! Right now there's no need for it, but may implement 
   in the future. 

*/

#include <Eigen/Core>

using Eigen::Matrix4f, Eigen::Vector3f, Eigen::Matrix3f;

class Object3D {
   private:
      Matrix4f current_transform;
      Matrix4f current_inverse;

      void update_inverse();
      unsigned int update_ref;

   public:
      Object3D() {
         current_transform = Matrix4f::Identity();
         current_inverse = Matrix4f::Identity();
         update_ref = 0;
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

      Vector3f transform_dir(Vector3f object_normal) {
         Matrix3f no_translation = get_transform_matrix().block<3,3>(0,0);
         return no_translation * object_normal;
      }


      Vector3f inv_affine(Vector3f& worldspace_vect) {
         Matrix4f inv = get_inverse_trans();
         Matrix3f aff_mat = inv.block<3,3>(0, 0);

         Vector3f inv_translation = Vector3f();
         inv_translation[0] = inv(0,3);
         inv_translation[1] = inv(1,3);
         inv_translation[2] = inv(2,3);

         Vector3f what = worldspace_vect+inv_translation;
         return aff_mat * what; // TODO: this right order?
      }

      // Apply various transformation to the object, saves the 
      // final state of the object. These are STATEFUL operations
      void apply_scale(float factor);

      // Rotate based on basic rotations - radians
      // The coordinate system is left-handed, with y pointing up
      // TODO: change to right-handed
      void apply_rotate(float rx, float ry, float rz);
      void apply_rotate_x(float rx);
      void apply_rotate_y(float ry);
      void apply_rotate_z(float rz);

      // Move the object
      void apply_translate(Vector3f dir, float mag);
      void apply_translate(Vector3f disp);

      // Apply general 3d transform
      void apply_transform(Matrix4f& trans);

      void reset_transform();

      // Linear interpolate translation.
      // TODO: implement rotational lerp using quaternions
      // TODO: not the really happy with the interface
      Object3D& lerp(Vector3f dest, float t);

};

#endif