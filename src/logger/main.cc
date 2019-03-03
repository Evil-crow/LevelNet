/**
 * Created by Crow on 3/3/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include <iostream>
#include <unistd.h>
#include "logger.h"

int main() {
  while (1) {
    LOG(WARN) << "dsada";
    LOG(INFO) << "senwmnwrtm";
    LOG(INFO) << "12";
    LOG(INFO) << 1243145L;
    sleep(5);
  }
}