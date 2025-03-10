#include <jsonfile.h>

#define TYPENAME JSONFile

////////////////////////////////////////////////////////////////////////////////
JSONFile *_(Construct)(const char *filename, AccessModes mode)
{  
  if (JSON_Construct(BASE(0))) {
    if (filename) {
      this->filename = malloc(strlen(filename) + 1);
      this->mode     = mode;

      strcpy((void*)this->filename, filename);
    }

    if (mode & ACCESS_READ) {
      CharStream *stream = (CharStream*) NEW (FileStream) (fopen(filename, "r"));

      if (stream) {
        JSON_Deserialize(BASE(0), stream);
        DELETE (stream);
      } else {
        THROW(NEW (Exception)("File not found!"));
      }
    }
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  if (this) {
    if (this->mode & ACCESS_WRITE) {
      CharStream *stream = (CharStream*) NEW (FileStream) (fopen(this->filename, "w+"));

      if (stream) {
        JSON_Serialize(BASE(0), stream);
        DELETE (stream);
      } else {
        THROW(NEW (Exception)("Couldn't open file!"));
      }
    }

    if (this->filename) {
      free((void*)this->filename);
      this->filename = NULL;
    }
      
    JSON_Destruct(BASE(0));
  }
}

#undef TYPENAME
