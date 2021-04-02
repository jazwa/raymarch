
/* Represents any sort of object inhabiting 3d space. Any sort of 
   linear transformations can be applied to these objects: rotations, 
   scaling, and translations. 
*/

#include <Eigen/Core>

using Eigen::Matrix4f, Eigen::Vector3f;

class Object3D {
   private:
      // Right now, all transformations are represented using 
      // 4x4 matrices. TODO: implement quaternions
      Matrix4f current_transform;

   public:
      Object3D() {
         current_transform = Maxtrix4f();
      }

      Matrix4f get_transform_matrix() {
         return current_transform;
      }

      // Get worldspace coordinates
      Eigen::Vector3f get_coords();


      // Apply various transformation to the object, saves the 
      // final state of the object. These are STATEFUL operations
      void apply_scale(float factor);
      void apply_rotate();
      void apply_translate();
      void apply_transform(Matrix4f trans);


      void reset_transform();

      // Linear interpolate translation.
      // TODO: implement rotational lerp using quaternions
      // TODO: not the really happy with the interface
      Object3D& lerp(Vector3f a, Vector3f b, float t);

};