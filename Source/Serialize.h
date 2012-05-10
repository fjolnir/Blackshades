#ifndef SERIALIZE_H
#define SERIALIZE_H

/* these all read big-endian data */

int ReadBool(int fd, int count, bool *b);
int ReadShort(int fd, int count, short *s);
int ReadInt(int fd, int count, int *s);
int ReadFloat(int fd, int count, float *f);
int ReadXYZ(int fd, int count, XYZ *xyz);
int ReadTexturedTriangle(int fd, int count, TexturedTriangle *tt);

#endif
