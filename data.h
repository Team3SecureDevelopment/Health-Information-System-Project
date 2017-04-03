#ifndef DATA_H
#define DATA_H

typedef struct patient* Patient;

void addNewPatient();

void findPatient();

Patient createPatient(char*, char*, char*, char*, int, int, char, char, char, char);

char *patientGetFirstName(Patient);

char *patientGetLastName(Patient);

char *patientGetDOB(Patient);

char *patientGetSocial(Patient);

int patientGetHeight(Patient);

int patientGetWeight(Patient);

int patientIsSmoker(Patient);

int patientHadSurgeries(Patient);

int patientMentalIllness(Patient);

int patientHasAllergies(Patient);

#endif
