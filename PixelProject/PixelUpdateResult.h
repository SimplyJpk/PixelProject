#pragma once

/**
 * Container Type\n
 * [0] = E_LogicResults\n
 * [1] = NeighbourType\n
 * [2] = NewPixel = E_PixelType (Local)\n
 * [3] = NewPixel = E_PixelType (Neighbour)\n
 * [4] = E_ChunkDirection (Pixel Update Direction)
 */
class PixelUpdateResult
{
private:
   int8_t results[5];
public:

   // Result
   inline E_LogicResults Result() { return static_cast<E_LogicResults>(results[0]); }
   inline void SetResult(const E_LogicResults result) { results[0] = result; }

   // Neighbour
   inline E_PixelType NeighbourType() { return static_cast<E_PixelType>(results[1]); }
   inline void SetNeighbour(const E_PixelType type) { results[1] = static_cast<int8_t>(type); }

   // Local NewPixel
   inline E_PixelType NewLocal() { return static_cast<E_PixelType>(results[2]); }
   inline void SetLocal(const E_PixelType type)
   {
      results[2] = static_cast<int8_t>(type);
      results[0] = static_cast<int8_t>(E_LogicResults::FirstReturnPixel);
   }

   // Neighbour NewPixel
   inline E_PixelType NewNeighbour() { return static_cast<E_PixelType>(results[3]); }
   inline void SetNewNeighbour(const E_PixelType type)
   {
      results[3] = static_cast<int8_t>(type);
      results[0] = static_cast<int8_t>(E_LogicResults::SecondReturnPixel);
   }

   // Direction
   inline E_ChunkDirection Dir() { return static_cast<E_ChunkDirection>(results[4]); }
   inline void SetDirection(const E_ChunkDirection dir) { results[4] = static_cast<int8_t>(dir); }


   // Set Local & Neighbour Type
   inline void SetLocalAndNeighbour(const E_PixelType local, const E_PixelType neighbour)
   {
      results[2] = static_cast<int8_t>(local);
      results[3] = static_cast<int8_t>(neighbour);
      results[0] = static_cast<int8_t>(E_LogicResults::DualReturnPixel);
   }

   // Simple one liner
   inline void Fail() { results[0] = static_cast<int8_t>(E_LogicResults::FailedUpdate); }
   inline void Pass() { results[0] = static_cast<int8_t>(E_LogicResults::SuccessUpdate); }
};