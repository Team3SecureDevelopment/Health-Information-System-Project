#ifndef DRAW_H
#define DRAW_H

void drawMenu(User);

void drawLogin();

void drawPatientSearch(FILE *fp);

void drawFilteredSearch(FILE *fp);

void drawPatientInfo();

void drawPatientNew();

void drawAppointment();

void drawAppointmentList(FILE *fp);

void drawLogs();

void drawViewUsers();

void drawAddUser();

void drawPassword();

void drawExit();

#endif
