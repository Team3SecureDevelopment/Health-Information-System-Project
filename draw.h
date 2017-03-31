#ifndef DRAW_H
#define DRAW_H

void drawMenu(User);

void drawLogin();

void drawPatientSearch(FILE *fp);

void drawPatientInfo(Patient);

void drawAppointment();

void drawAppointmentList(FILE *fp);

void drawLogs();

void drawExit();

#endif
