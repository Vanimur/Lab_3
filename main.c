#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char *convertStrToLongBv(char *str, size_t *cells){
    if (str == NULL || cells == NULL) return NULL;
    size_t len = 0, ix = 0;
    len = strlen(str);

    if (len == 0) {
        *cells = 0;
        return NULL;
    }

    *cells = ((len - 1) / 8) + 1;

    unsigned char *vec = (unsigned char *)calloc(*cells, sizeof(unsigned char));
    if (vec == NULL) return NULL;

    unsigned char mask;
    for (size_t i = 0; i < *cells; i++) {
        mask = 1;
        for (int j = 0; (j < 8) && (ix < len); j++){
            if (str[ix] != '0'){
                vec[i] = vec[i] | mask;
            }
            mask = mask << 1;
            ix++;
        }
    }
    return vec;
}


unsigned char *convertLongBvToStr(unsigned char *vec, size_t cells){
    size_t ix = 0;
    if (vec == NULL) return NULL;

    if (cells == 0) {
        return NULL;
    }

    size_t len = 8 * cells + 1;
    unsigned char *str = (unsigned char*)calloc(len, sizeof(unsigned char));
    unsigned char mask;
    for (size_t i = 0; i < cells; i++){
        mask = 1;
        for (int j = 0; (j < 8) && (ix < len); j++){
            if ((vec[i] & mask) != 0)
                str[ix] = '1';
            else
                str[ix] = '0';
            mask = mask << 1;
            ix++;
        }
    }
    return str;
}


void set_bit_1(unsigned char *vec, size_t len, size_t k){
    if (vec == NULL) return;
    size_t byte = 0, bit =0;
    unsigned char mask = 1;

    if (k >= len) return;

    byte = k / 8;
    bit = k % 8;

    mask = mask << bit;

    vec[byte] = vec[byte] | mask;
}


void set_bit_0(unsigned char *vec, size_t len, size_t k){
    if (vec == NULL) return;
    size_t byte = 0, bit =0;
    unsigned char mask = 1;

    if (k >= len) return;

    byte = k / 8;
    bit = k % 8;

    mask = ~(mask << bit);

    vec[byte] = vec[byte] & mask;
}


unsigned char *log_Mul(unsigned char *vecA, size_t lenA, unsigned char *vecB, size_t lenB){
    if (vecA == NULL || vecB == NULL) return NULL;
    if (lenA != lenB) return NULL;
    if (lenA == 0) return NULL;

    size_t cells = (lenA + 7) / 8;

    unsigned char *result = (unsigned char*)calloc(cells, sizeof(unsigned char));
    if (result == NULL) return NULL;

    for (size_t i = 0; i < cells; i++){
        result[i] = vecA[i] & vecB[i];
    }
    return result;
}


unsigned char *log_Sub(unsigned char *vecA, size_t lenA, unsigned char *vecB, size_t lenB){
    if (vecA == NULL || vecB == NULL) return NULL;
    if (lenA != lenB) return NULL;
    if (lenA == 0) return NULL;

    size_t cells = (lenA + 7) / 8;

    unsigned char *result = (unsigned char*)calloc(cells, sizeof(unsigned char));
    if (result == NULL) return NULL;

    for (size_t i = 0; i < cells; i++){
        result[i] = vecA[i] | vecB[i];
    }
    return result;
}


unsigned char *log_Xor(unsigned char *vecA, size_t lenA, unsigned char *vecB, size_t lenB){
    if (vecA == NULL || vecB == NULL) return NULL;
    if (lenA != lenB) return NULL;
    if (lenA == 0) return NULL;

    size_t cells = (lenA + 7) / 8;

    unsigned char *result = (unsigned char*)calloc(cells, sizeof(unsigned char));
    if (result == NULL) return NULL;

    for (size_t i = 0; i < cells; i++){
        result[i] = vecA[i] ^ vecB[i];
    }
    return result;
}


unsigned char *log_Not(unsigned char *vec, size_t len){
    if (vec == NULL) return NULL;
    if (len == 0) return NULL;

    size_t cells = (len + 7) / 8;

    unsigned char *result = (unsigned char*)calloc(cells, sizeof(unsigned char));
    if (result == NULL) return NULL;

    for (size_t i = 0; i < cells; i++){
        result[i] = ~vec[i];
    }

    size_t xvost_bit = len % 8;
    if (xvost_bit != 0){
        unsigned char mask = -1;
        mask = (mask >> (8 - xvost_bit));
        result[cells - 1] = result[cells - 1] & mask;
    }

    return result;
}


unsigned char *log_Shift_Left(unsigned char *vec, size_t len, size_t k) {
    if (vec == NULL) return NULL;
    if (len == 0) return NULL;

    size_t cells = (len + 7) / 8;
    unsigned char *result = (unsigned char*)calloc(cells, sizeof(unsigned char));
    if (result == NULL) return NULL;

    if (k >= len) return result;

    size_t byte_shift = k / 8;
    size_t bit_shift = k % 8;

    unsigned char current;
    unsigned char transfer;

    for (size_t i = 0; i < cells; i++) {
        current = 0;

        if (i + byte_shift < cells) {
            current = vec[i + byte_shift] >> bit_shift;
        }

        if (bit_shift > 0 && (i + byte_shift + 1 < cells)) {
            transfer = vec[i + byte_shift + 1] << (8 - bit_shift);
            current = current | transfer;
        }

        result[i] = current;
    }

    size_t xvost_bit = len % 8;
    if (xvost_bit != 0) {
        unsigned char mask = -1;
        mask = (mask >> (8 - xvost_bit));
        result[cells - 1] = result[cells - 1] & mask;
    }

    return result;
}


unsigned char *log_Shift_Right(unsigned char *vec, size_t len, size_t k) {
    if (vec == NULL) return NULL;
    if (len == 0) return NULL;

    size_t cells = (len + 7) / 8;
    unsigned char *result = (unsigned char*)calloc(cells, sizeof(unsigned char));
    if (result == NULL) return NULL;

    if (k >= len) return result;

    size_t byte_shift = k / 8;
    size_t bit_shift = k % 8;

    unsigned char current;
    unsigned char transfer;

    for (size_t i = cells - 1; i < cells; i--) {
        current = 0;

        if (i >= byte_shift) {
            current = vec[i - byte_shift] << bit_shift;
        }

        if (bit_shift > 0 && (i > byte_shift)) {
            transfer = vec[i - byte_shift - 1] >> (8 - bit_shift);
            current = current | transfer;
        }

        result[i] = current;
    }

    size_t xvost_bit = len % 8;
    if (xvost_bit != 0) {
        unsigned char mask = -1;
        mask = (mask >> (8 - xvost_bit));
        result[cells - 1] = result[cells - 1] & mask;
    }

    return result;
}


void print_vec(unsigned char *vec, size_t len) {
    if (vec == NULL) return;
    size_t ix = 0;
    size_t cells = (len + 7) / 8;
    for (size_t i = 0; i < cells; i++){
        unsigned char mask = 1;
        for (int j = 0; (j < 8) && (ix < len); j++){
            if ((vec[i] & mask) != 0)
                printf("1");
            else
                printf("0");
            mask = mask << 1;
            ix++;
        }
    }
    printf("\n");
}

int main()
{
    {
      char str[20] = "abcdefg11h";
      //char str[20] = "0000000000";
      //char str[20] = "1";
      //char str[20] = "0";
      //char str[20] = "";
      size_t vector_cells;
      size_t bit_len = strlen(str);

      unsigned char *vector = convertStrToLongBv(str, &vector_cells);
      //unsigned char *vector = convertStrToLongBv(NULL, &vector_cells);
      //unsigned char *vector = convertStrToLongBv(str, NULL);
      unsigned char *str_vector = convertLongBvToStr(vector, vector_cells);
      //unsigned char *str_vector = convertLongBvToStr(NULL, vector_cells);
      //unsigned char *str_vector = convertLongBvToStr(vector, 0);
      printf("Vector: ");
      print_vec(vector, bit_len);
      printf("Str vector: %s\n", str_vector);

      free(vector);
      vector = NULL;
      free(str_vector);
      str_vector = NULL;
    }



    //    Установка битов
    {
      char str[20] = "abcdefg11h";
      size_t vector_cells;
      size_t bit_len = strlen(str);

      unsigned char *vector = convertStrToLongBv(str, &vector_cells);
      //set_bit_1(vector, bit_len, 3); // 0, 10, 11
      set_bit_0(vector, bit_len, 3); // 0, 9, 10, 11
      //set_bit_0(vector, 0, 1);
      //set_bit_0(NULL, bit_len, 3);
      unsigned char *str_vector = convertLongBvToStr(vector, vector_cells);
      printf("Set_bit: %s\n", str_vector);

      free(vector);
      vector = NULL;
      free(str_vector);
      str_vector = NULL;
    }

    //        Сложение / Умножение / XOR
    {
      char strA[20] = "10101001";
      char strB[20] = "01010101";
      size_t vecA_cells;
      size_t vecB_cells;
      size_t bit_lenA = strlen(strA);
      size_t bit_lenB = strlen(strB);

      unsigned char *vecA = convertStrToLongBv(strA, &vecA_cells);
      unsigned char *vecB = convertStrToLongBv(strB, &vecB_cells);

      unsigned char *Sub = log_Sub(vecA, bit_lenA, vecB, bit_lenB);
      //unsigned char *Sub = log_Sub(NULL, bit_lenA, vecB, bit_lenB);
      //unsigned char *Sub = log_Sub(vecA, bit_lenA, NULL, bit_lenB);
      printf("Sub: ");
      print_vec(Sub, bit_lenA);

      unsigned char *Mul = log_Mul(vecA, bit_lenA, vecB, bit_lenB);
      printf("Mul: ");
      print_vec(Mul, bit_lenA);

      unsigned char *XOR = log_Xor(vecA, bit_lenA, vecB, bit_lenB);
      printf("XOR: ");
      print_vec(XOR, bit_lenA);

      free(vecA);
      free(vecB);
      vecA = NULL;
      vecB = NULL;
      free(Sub);
      free(Mul);
      free(XOR);
      XOR = NULL;
      Sub = NULL;
      Mul = NULL;
    }

    //      Отрицание NOT
    {
      char str[20] = "000011111";
      size_t vector_cells;
      size_t bit_len = strlen(str);

      unsigned char *vector = convertStrToLongBv(str, &vector_cells);
      unsigned char *Not = log_Not(vector, bit_len);
      //unsigned char *Not = log_Not(NULL, bit_len);
      unsigned char *str_vector = convertLongBvToStr(Not, vector_cells);
      printf("Not: %s\n", str_vector);

      free(vector);
      vector = NULL;
      free(Not);
      Not = NULL;
      free(str_vector);
      str_vector = NULL;
    }

    //        Сдвиги
    {
      char str[20] = "111111111";
      size_t vector_cells;
      size_t bit_len = strlen(str);

      unsigned char *vector = convertStrToLongBv(str, &vector_cells);
      unsigned char *left_shift = log_Shift_Left(vector, bit_len, 5); // 0, 1, 7, 8, 9
      printf("Base left shift: ");
      print_vec(left_shift, bit_len);

      unsigned char *right_shift = log_Shift_Right(vector, bit_len, 5); // 0, 1, 7, 8, 9
      printf("Base right shift: ");
      print_vec(right_shift, bit_len);


      free(vector);
      vector = NULL;
      free(left_shift);
      left_shift = NULL;
      free(right_shift);
      right_shift = NULL;
    }

    //        Большие Сдвиги
    {
      char str[101] = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
      size_t vector_cells;
      size_t bit_len = strlen(str);

      unsigned char *vector = convertStrToLongBv(str, &vector_cells);
      unsigned char *Not = log_Not(vector, bit_len);
      for (size_t i = 0; i <= 100; i++){
          unsigned char *left_shift = log_Shift_Left(Not, bit_len, i);
          //print_vec(left_shift, bit_len);
          free(left_shift);
          left_shift = NULL;
      }

      printf("------------------------------------\n");

      for (size_t i = 0; i <= 100; i++){
          unsigned char *right_shift = log_Shift_Right(Not, bit_len, i);
          //print_vec(right_shift, bit_len);
          free(right_shift);
          right_shift = NULL;
      }

      free(vector);
      vector = NULL;
      free(Not);
      Not = NULL;
    }


    //       Большие установки
    {
      char str[101] = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
      size_t vector_cells;
      size_t bit_len = strlen(str);

      unsigned char *vector = convertStrToLongBv(str, &vector_cells);
      for (size_t i = 0; i < 100; i++){
          set_bit_1(vector, bit_len, i);
          //print_vec(vector, bit_len);
      }

      printf("------------------------------------\n");

      for (size_t i = 0; i < 100; i++){
          set_bit_0(vector, bit_len, i);
          //print_vec(vector, bit_len);
      }

      free(vector);
      vector = NULL;
    }
    return 0;
}
