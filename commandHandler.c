#include <stdio.h>

#define using_more
#include <more/common.h>
#include <more/fields.h>

// Returns: an error message on error, otherwise NULL
string handleCommand(string command)
{
  Text text;
  FieldToken field;

  Text_init(catch_Text, &text, command);

  while(1) {
    Text_parseField(catch_Text, &text, &field, false);
    if(FieldToken_eof(&field)) {
      break;
    }

    printf("[DEBUG] handleCommand: found string '%s'\n", field.text.ptr);

  }


  return (more_string){0, 0};
 catch_Text:
  return text.error.message;
}

