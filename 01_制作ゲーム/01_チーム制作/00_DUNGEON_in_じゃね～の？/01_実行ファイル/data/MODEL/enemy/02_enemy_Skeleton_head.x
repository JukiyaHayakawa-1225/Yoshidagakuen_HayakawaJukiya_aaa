xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 28;
 3.24438;0.01912;-3.24438;,
 -3.24438;0.01912;-3.24438;,
 -3.24438;6.50783;-3.24438;,
 3.24438;6.50783;-3.24438;,
 3.24438;6.50783;-2.16292;,
 -3.24438;6.50783;-2.16292;,
 -3.24438;6.50783;3.24438;,
 3.24438;6.50783;3.24438;,
 -3.24438;0.01912;3.24438;,
 3.24438;0.01912;3.24438;,
 3.24438;0.01912;3.24438;,
 -3.24438;0.01912;3.24438;,
 -3.24438;0.01912;-2.16292;,
 3.24438;0.01912;-2.16292;,
 -3.24438;6.50783;-2.16292;,
 -3.24438;0.01912;-2.16292;,
 -3.24438;0.01912;3.24438;,
 -3.24438;6.50783;3.24438;,
 3.24438;0.01912;3.24438;,
 3.24438;0.01912;-2.16292;,
 3.24438;6.50783;-2.16292;,
 3.24438;6.50783;3.24438;,
 3.24438;-0.84714;-2.16292;,
 -3.24438;-0.84714;-2.16292;,
 -3.24438;-0.84714;-3.24438;,
 3.24438;-0.84714;-3.24438;,
 -3.24438;-0.84714;-2.16292;,
 3.24438;-0.84714;-2.16292;;
 
 14;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;7,6,8,9;,
 4;10,11,12,13;,
 4;14,15,16,17;,
 4;18,19,20,21;,
 4;3,2,5,4;,
 4;1,15,14,2;,
 4;22,23,24,25;,
 4;20,19,0,3;,
 4;13,12,23,22;,
 4;15,1,24,26;,
 4;1,0,25,24;,
 4;0,19,27,25;;
 
 MeshMaterialList {
  1;
  14;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/skeleton.png";
   }
  }
 }
 MeshNormals {
  6;
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;;
  14;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;1,1,1,1;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;5,5,5,5;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;5,5,5,5;;
 }
 MeshTextureCoords {
  28;
  0.428570;0.357140;,
  0.214290;0.357140;,
  0.214290;0.571430;,
  0.428570;0.571430;,
  0.428570;0.607140;,
  0.214290;0.607140;,
  0.214290;0.785710;,
  0.428570;0.785710;,
  0.214290;1.000000;,
  0.428570;1.000000;,
  0.428570;0.000000;,
  0.214290;0.000000;,
  0.214290;0.178570;,
  0.428570;0.178570;,
  0.178570;0.571430;,
  0.178570;0.357140;,
  0.000000;0.357140;,
  0.000000;0.571430;,
  0.642860;0.357140;,
  0.464290;0.357140;,
  0.464290;0.571430;,
  0.642860;0.571430;,
  0.428570;0.250000;,
  0.214290;0.250000;,
  0.214290;0.285710;,
  0.428570;0.285710;,
  0.178570;0.285710;,
  0.464290;0.285710;;
 }
}
