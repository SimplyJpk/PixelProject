#pragma once

enum E_ChunkDirection : int
{
   North = 0,
   NorthEast = 1,
   East = 2,
   SouthEast = 3,
   South = 4,
   SouthWest = 5,
   West = 6,
   NorthWest = 7,
   DIR_COUNT = 8
};

/// <summary>
/// Chunks are updated in a few pieces to reduce chunk-bleeding, the enum states represent which side of the array each is.
/// </summary>
enum E_ChunkUpdatePiece
{
   NorthChunkPiece = 0,
   EastChunkPiece = 1,
   WestChunkPiece = 2,
   SouthChunkPiece = 3,
   CenterChunkPiece = 4
};
