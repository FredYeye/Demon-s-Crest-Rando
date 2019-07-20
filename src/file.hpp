#pragma once


const std::vector<uint8_t> FileToU8Vec(const std::string inFile);
void U8vecToFile(const std::vector<uint8_t> &outVec, const std::string outFile);
