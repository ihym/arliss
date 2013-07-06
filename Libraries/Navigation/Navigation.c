void move_forwards(int speedpinA,int speedpinB,int pinI1A,int pinI2A,int pinI1B,int pinI2B){
    digitalWrite(pinI1A,LOW);// DC motor A rotates clockwise
    digitalWrite(pinI2A,HIGH);
    digitalWrite(pinI1B,LOW);// DC motor B rotates clockwise
    digitalWrite(pinI2B,HIGH);
    analogWrite(speedpinA,100);//input a value to set the speed
    analogWrite(speedpinB,100);
    delay(2000);
}

void move_backwards(int speedpinA,int speedpinB,int pinI1A,int pinI2A,int pinI1B,int pinI2B){
    digitalWrite(pinI1A,HIGH);// DC motor A rotates anticlockwise
    digitalWrite(pinI2A,LOW);
    digitalWrite(pinI1B,HIGH);// DC motor B rotates anticlockwise
    digitalWrite(pinI2B,LOW);
    analogWrite(speedpinA,50);//input a value to set the speed
    analogWrite(speedpinB,50);
    delay(1000);
}

void turn_left(int speedpinA,int speedpinB,int pinI1A,int pinI2A,int pinI1B,int pinI2B){
    digitalWrite(pinI1A,LOW); // DC motor A rotates clockwise
    digitalWrite(pinI2A,HIGH);
    digitalWrite(pinI1B,LOW); // DC motor B rotates clockwise
    digitalWrite(pinI2B,HIGH);
    analogWrite(speedpinA,98); // different speed values to set the desired angle/if 255->234
    analogWrite(speedpinB,100);

    delay(200);

    analogWrite(speedpinA,100);
    analogWrite(speedpinB,100);

    delay(1000);
}

void turn_right(int speedpinA,int speedpinB,int pinI1A,int pinI2A,int pinI1B,int pinI2B){
    digitalWrite(pinI1A,LOW); // DC motor A rotates clockwise
    digitalWrite(pinI2A,HIGH);
    digitalWrite(pinI1B,LOW); // DC motor B rotates clockwise
    digitalWrite(pinI2B,HIGH);
    analogWrite(speedpinA,100); // different speed values to set the desired angle/if 255->234
    analogWrite(speedpinB,98);

    delay(200);

    analogWrite(speedpinA,100);
    analogWrite(speedpinB,100);

    delay(1000);
}

void stop_rotating(int pinI1A,int pinI2A,int pinI1B,int pinI2B){
    digitalWrite(pinI1A,HIGH);// DC motor A stop rotating
    digitalWrite(pinI2A,HIGH);
    digitalWrite(pinI1B,HIGH);// DC motor B stop rotating
    digitalWrite(pinI2B,HIGH);

    delay(5000);
}
