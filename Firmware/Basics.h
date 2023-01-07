/*************************************************************************
* FIREKEY-PROJECT
* 
* TODO comment
*************************************************************************/

// TODO check if file is needed?

/**
 * Converts a color hex value into separate red, green and blue values.
 */
void hexToRGB(String hexCode, int& r, int& g, int& b) {
  // remove the '#' character from the hex code if it exists
  hexCode.replace("#", "");

  // check if hex code has 6 or 3 characters
  if (hexCode.length() == 6) {
    // parse the 6 character hex code
    r = strtol(hexCode.substring(0, 2).c_str(), NULL, 16);
    g = strtol(hexCode.substring(2, 4).c_str(), NULL, 16);
    b = strtol(hexCode.substring(4, 6).c_str(), NULL, 16);
  } else if (hexCode.length() == 3) {
    // parse the 3 character hex code
    String hexCodeR = hexCode.substring(0, 1);
    hexCodeR += hexCodeR;
    r = strtol(hexCodeR.c_str(), NULL, 16);

    String hexCodeG = hexCode.substring(1, 2);
    hexCodeG += hexCodeG;
    g = strtol(hexCodeG.c_str(), NULL, 16);

    String hexCodeB = hexCode.substring(2, 3);
    hexCodeB += hexCodeB;
    b = strtol(hexCodeB.c_str(), NULL, 16);
  }
}
// TODO check if needed here or in configurator software?

/**
 * Copies a string from the program memory into the destination.
 */
void getProgMemStr(const char str[], char dest[]) {
  char c;
  byte i = 0;
  while ((c = pgm_read_byte(str++))) {
    dest[i] = c;
    i++;
  }
  dest[i] = '\0';
}