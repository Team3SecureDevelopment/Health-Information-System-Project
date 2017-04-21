#ifndef DATA_H
#define DATA_H

typedef struct patient* Patient;

void addNewPatient();

void findPatient();

Patient createPatient(char*, char*, char*, char*, int, int, char, char, char, char, char);

void setAllergyInfo(int);

void getAllergyInfo(int);

void setPrescriptionInfo(int);

void getPrescriptionInfo(int);

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

int patientOnPrescriptions(Patient);

void filteredSearch();

#endif
