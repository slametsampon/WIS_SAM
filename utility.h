/*
  utility.h - Library for DHT wrapper
  By : Sam Sept 04, 2021

  functions : see following function in detail....
*/

#ifndef utility_h
#define utility_h

#include <Arduino.h>
#include <LittleFS.h>

const String PATH_ROOT = "/";
const String FILE_USER = "users.cfg";

const String PATH_LS = "/logsheet/";

class FileSystem
{
public:
  FileSystem(String);
  void listFilesInDir(String); //list files in all dir's
  void append(const char *path, const char *message);
  void write(const char *path, const char *message);
  String read(const char *path);
  File readStream(const char *path);

private:
  String _id;
};
void displaySerial(String, int);
void displaySerial(String, float);
void displaySerial(String, String);
void blink(unsigned long, int);

#endif