#include <WiFiClientSecure.h>
#include <base64.h>
#include "Mail.h"

//#define NDEBUG

static inline bool checkResponse(const String& response, int16_t code) {
  return response.startsWith(String(code) + ' ');
}

static bool checkResponse(const String& response, int16_t code, const String& errorMsg) {
  bool result = checkResponse(response, code);

  if (! result) {
    Serial.print(errorMsg);
    Serial.print(F(": \""));
    if (response.endsWith(F("\r\n")))
      Serial.print(response.substring(0, response.length() - 2));
    else
      Serial.print(response);
    Serial.println(F("\"!"));
  }

  return result;
}

static String getLine(WiFiClient& client) {
  String result;

  if (client.connected())
    result = client.readStringUntil('\n');
#ifndef NDEBUG
  Serial.print(F("<<"));
  Serial.println(result);
#endif

  return result;
}

static void sendLine(WiFiClient& client, const String& str) {
#ifndef NDEBUG
  Serial.print(F(">>"));
  Serial.println(str);
#endif
  if (client.connected())
    client.println(str);
}

static String sendCommand(WiFiClient& client, const String& str) {
  sendLine(client, str);
  client.flush();

  return getLine(client);
}

bool sendMail(const String& smtpHost, uint16_t smtpPort, const String& smtpUser, const String& smtpPassword, const String& mailTo, const String& mailSubject, const String& mailMessage) {
  WiFiClientSecure client;
  String str;

  Serial.print(F("Connecting to "));
  Serial.println(smtpHost);
  client.setInsecure();
  if (! client.connect(smtpHost.c_str(), smtpPort)) {
    Serial.println(F("Connection failed!"));
    return false;
  }
  client.setTimeout(15000); // 15 sec. read timeout
  str = getLine(client);
  if (! checkResponse(str, 220, F("SMTP connection error")))
    return false;
  str = sendCommand(client, F("HELO esp8266.local"));
  if (! checkResponse(str, 250, F("SMTP HELO error")))
    return false;
  if (smtpPassword.length()) {
    str = sendCommand(client, F("AUTH LOGIN"));
    if (! checkResponse(str, 334, F("SMTP AUTH error")))
      return false;
    str = sendCommand(client, base64::encode(smtpUser));
    if (! checkResponse(str, 334, F("SMTP AUTH USER error")))
      return false;
    str = sendCommand(client, base64::encode(smtpPassword));
    if (! checkResponse(str, 235, F("SMTP AUTH PASS error")))
      return false;
  }
  str = F("MAIL FROM: <");
  str += smtpUser;
  str += '>';
  str = sendCommand(client, str);
  if (! checkResponse(str, 250, F("SMTP MAIL FROM error")))
    return false;
  str = F("RCPT TO: <");
  str += mailTo;
  str += '>';
  str = sendCommand(client, str);
  if (! checkResponse(str, 250, F("SMTP RCPT TO error")))
    return false;
  str = sendCommand(client, F("DATA"));
  if (! checkResponse(str, 354, F("SMTP DATA error")))
    return false;
  str = F("Subject: =?utf-8?B?");
  str += base64::encode(mailSubject);
  str += F("=?=");
  sendLine(client, str);
  sendLine(client, F("MIME-Version: 1.0"));
  sendLine(client, F("Content-type: text/plain; charset=utf-8"));
  str = F("From: <");
  str += smtpUser;
  str += '>';
  sendLine(client, str);
  str = F("To: <");
  str += mailTo;
  str += '>';
  sendLine(client, str);
  sendLine(client, "");
  sendLine(client, mailMessage);
  str = sendCommand(client, F("\r\n."));
  if (! checkResponse(str, 250, F("SMTP send error")))
    return false;
  sendCommand(client, F("QUIT"));
  client.stop();

  return true;
}
