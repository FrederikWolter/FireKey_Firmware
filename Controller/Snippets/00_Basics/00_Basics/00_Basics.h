void hexToRGB(String hexCode, int& r, int& g, int& b) {
  // Remove the '#' character from the hex code if it exists
  hexCode.replace("#", "");

  // Check if the hex code has 6 or 3 characters
  if (hexCode.length() == 6) {
    // Parse the 6 character hex code
    r = strtol(hexCode.substring(0, 2).c_str(), NULL, 16);
    g = strtol(hexCode.substring(2, 4).c_str(), NULL, 16);
    b = strtol(hexCode.substring(4, 6).c_str(), NULL, 16);
  } else if (hexCode.length() == 3) {
    // Parse the 3 character hex code
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
