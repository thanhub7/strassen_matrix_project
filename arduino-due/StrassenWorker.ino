int A[2][2];
int B[2][2];
int C[2][2];

void setup() {
  Serial1.begin(9600); // UART with Raspberry Pi
  Serial.begin(9600);  // USB Serial for debug
  while (!Serial);
  Serial.println("Arduino ready");
}

void loop() {
  if (Serial1.available()) {
    String tag = Serial1.readStringUntil('\n');
    tag.trim();
    Serial.print("Received tag: ");
    Serial.println(tag);

    readMatrix(Serial1, A, "A");
    readMatrix(Serial1, B, "B");

    multiply(A, B, C);
    sendResult(Serial1, C);
  }
}

void readMatrix(HardwareSerial& s, int M[2][2], const char* label) {
  Serial.print("Receiving matrix ");
  Serial.println(label);

  for (int i = 0; i < 2; ++i) {
    while (!s.available());
    String line = s.readStringUntil('\n');
    line.trim();
    Serial.print("Line ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(line);

    int idx = 0;
    char *ptr = strtok((char*)line.c_str(), " ");
    while (ptr != NULL && idx < 2) {
      M[i][idx++] = atoi(ptr);
      ptr = strtok(NULL, " ");
    }
  }

  Serial.print("Matrix ");
  Serial.println(label);
  for (int i = 0; i < 2; ++i) {
    Serial.print("[ ");
    for (int j = 0; j < 2; ++j) {
      Serial.print(M[i][j]);
      Serial.print(" ");
    }
    Serial.println("]");
  }
}

void multiply(int A[2][2], int B[2][2], int C[2][2]) {
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      C[i][j] = 0;
      for (int k = 0; k < 2; ++k) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

void sendResult(HardwareSerial& s, int C[2][2]) {
  s.println("RESULT");
  for (int i = 0; i < 2; ++i) {
    s.print(C[i][0]);
    s.print(" ");
    s.println(C[i][1]);
  }
  Serial.println("Sent RESULT to Pi.");
}
