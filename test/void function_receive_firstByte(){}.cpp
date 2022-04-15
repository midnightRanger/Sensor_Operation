void function_receive_firstByte(){}
  //Здесь должна быть функция для приёма 1 байта адреса
  //...
  //Парсинг байт. Каждая переманная - каждый отдайльный байт: firstByte, secondByte, threeByte
  int FirstByte;
  dali_address_rxtx(FirstByte, SecondByte, ThreeByte); //Сюда нужно вставить firstByte на этапе приёма первого байта
}
int function_receive_SecondByte(){
  //Здесь должна быть функция для приёма 2 байта комманды
  //...
  //Парсинг байт. Каждая переманная - каждый отдайльный байт: firstByte, secondByte, threeByte
  int SecondByte;
  return SecondByte; //Сюда нужно вставить secondByte на этапе приёма второго байта.
}
int function_receive_ThreeByte(){
  //Здесь должна быть функция для приёма 3 байта значения(value)
  //...
  //Парсинг байт. Каждая переманная - каждый отдайльный байт: firstByte, secondByte, threeByte
  int ThreeByte;
  return ThreeByte;//Сюда нужно вставить threeByte на этапе приёма 3 команды в функции switch
}

/*
 * Когда-нибудь здесь будет пояснение по тем номерам, что возвращает функция.
 * Пока методика такая: Если ответ меньше 32 то всё хорошо
 */

int dali_address_rxtx(int FirstByte, int SecondByte, int ThreeByte){
    switch(FirstByte) {
        case 255: { //0b 1111 1111
            if(check_flags() == 1){ //Оптимизация кода, убрана лишняя переменная
                dali_command_txrx(SecondByte, ThreeByte);
                return 1;
            }
        }
        case 254: { //0b 1111 1110
            if(check_flags() == 1){
                dim_powersupply(SecondByte);
                return 2;
            }
        }
        case 253: {
             if(check_flags() == 1){
                dali_command_txrx(SecondByte, ThreeByte);
                return 3;
            }
        }
        case 252: {
            if(check_flags() == 1){
                //int SecondByte = function_receive_SecondByte();
                dim_powersupply(SecondByte);
                return 4;
            }
        }

    }

  if(FirstByte >= 0 && FirstByte < 128){
    status_check_flags = check_flags();
      if (status_check_flags == 1){
        int new_FirstByte = FirstByte % 2;
        if(new_FirstByte == 0){
          int new_FirstByte_check = FirstByte / 2;
          int short_adr_memory_read = 1; //ADD READ SHORTADR
          //int short_adr_memory_read = EEPROM.read(0); //Чтение из памяти своего shortAdr
          
          if(short_adr_memory_read == new_FirstByte_check){//FIX LOGIC
            dim_powersupply(SecondByte);    
            return 0;
          }
        }
        if(new_FirstByte == 1){
          int new_FirstByte_check = FirstByte - 1;
          new_FirstByte_check = new_FirstByte_check / 2;
          int short_adr_memory_read = 1; //ADD READ SHORTADR
          //int short_adr_memory_read = EEPROM.read(0);
          if(short_adr_memory_read == new_FirstByte_check){ //FIX LOGIC
            dali_command_txrx(SecondByte, ThreeByte);   
            return 5;
          }
          return 242;  
        }
      return 241;
    }    
  return 240;
  }    

  if(FirstByte >= 128 && FirstByte < 160){
  status_check_flags = check_flags();
  if (status_check_flags == 1){
  int new_FirstByte = FirstByte % 2;
  if(new_FirstByte == 0){
  int new_FirstByte_check = (FirstByte - 128) / 2;
  int short_adr_memory_read = 2; //Чтение из памяти своего Group
  
  if(short_adr_memory_read == new_FirstByte_check){ //FIX LOGIC
  dim_powersupply(SecondByte);  
  return 5;
  }
  }
  if(new_FirstByte == 1){
  int new_FirstByte_check  = (FirstByte - 128) - 1;
  new_FirstByte_check  = new_FirstByte_check  / 2;
  int short_adr_memory_read = 2; //Чтение из памяти своего Group 
  if(short_adr_memory_read == new_FirstByte_check){ //FIX LOGIC
  dali_command_txrx(SecondByte, ThreeByte);   
  return 6;
  }
  return 254;   
  }
  return 241;
  }    
  return 240;
  }

  //Special command
  if(FirstByte >= 160 && FirstByte < 204){

  if (FirstByte == 165){
    F_Init = 1;
    Init_slave();
    Serial.println("is");
  return 165;
  }    
  return 240; 
  }

  //Reserved
  if(FirstByte >= 204 && FirstByte < 252){ 
  int SecondByte = function_receive_SecondByte(); 
  status_check_flags = check_flags();
  if (status_check_flags == 1){
  int new_SecondByte = SecondByte % 2;
  if(new_SecondByte == 0){
    return 9;
  }
  if(new_SecondByte == 1){
    //todo any function from Trion(Сами придумать можем если надо)
    return 10;
  }
  }     
  return 240;
  }
  return 255;
  }
