#include "utility.h"

FileSystem::FileSystem(String id) : _id(id) {}

void FileSystem::listFilesInDir(String dir_path)
{
    Dir dir = LittleFS.openDir(dir_path);
    while (dir.next())
    {
        if (dir.isFile())
        {
            // print file names
            Serial.print("File: ");
            Serial.println(dir_path + dir.fileName());
        }
        if (dir.isDirectory())
        {
            // print directory names
            Serial.print("Dir: ");
            Serial.println(dir_path + dir.fileName() + "/");
            // recursive file listing inside new directory
            listFilesInDir(dir_path + dir.fileName() + "/");
        }
    }
}

void FileSystem::append(const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = LittleFS.open(path, "a");
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.print("message : ");
        Serial.print(message);
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();
}

void FileSystem::write(const char *path, const char *message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = LittleFS.open(path, "w");
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.print("message : ");
        Serial.print(message);
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
    delay(2000); // Make sure the CREATE and LASTWRITE times are different
}

String FileSystem::read(const char *path)
{
    String message = "";
    Serial.printf("Reading file: %s\n", path);

    File file = LittleFS.open(path, "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return message;
    }

    int c = 0;
    Serial.println("Contents : ");
    while (file.available())
    {
        c = file.read();
        message += (char)c;
    }
    Serial.println(message);
    file.close();

    return message;
}

void displaySerial(String label, String msg)
{
    String str = String(label + " : ");
    str = String(str + msg);
    Serial.println(str);
}

void displaySerial(String label, float value)
{
    String strValue = String(value);
    displaySerial(label, strValue);
}

void displaySerial(String label, int value)
{
    displaySerial(label, (float)value);
}
