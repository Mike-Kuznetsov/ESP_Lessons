#ifndef __MAIL_H
#define __MAIL_H

bool sendMail(const String& smtpHost, uint16_t smtpPort, const String& smtpUser, const String& smtpPassword, const String& mailTo, const String& mailSubject, const String& mailMessage);

#endif
