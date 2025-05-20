#ifndef PROFESSORS_H
#define PROFESSORS_H

struct Course {
  String name;
  String id;
};

struct Professor {
  String name;
  Course courses[2];
};

Professor professors[] = {
  {"Dr. Aditi Shinde", {{"MOC", "R4EC4012S"}, {"ES", "R4ET4011S"}}},
  {"Dr. R.A. Patil", {{"ACT", "R4ET4013S"}, {"Sat Com", "R4ET4012S"}}},
  {"Dr. Faruk A.S. Kazi", {{"AMLFS", "R4ET4122P"}, {"MSD", "R4EC4011S"}}},
  {"Dr. Neha Mishra", {{"WCS", "R4EC4013S"}, {"NLP", "R4EC4123T"}}}
};

#endif
