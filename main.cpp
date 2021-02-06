/*
  susjoiner v0.1

  Copyright (C) 2021  inonote

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>

using namespace std;

char ito36(uint32_t i) {
  if (i > 35)
    return 0;
  return i < 10 ? '0' + i : 'A' + i - 10;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cout
        << "susjoiner v0.1 -- Copyright (C) 2021  inonote" << endl
        << endl
        << "Usage: susjoiner <base SUS file>" << endl
        << endl
        << "Readme: https://github.com/inonote/susjoiner/blob/master/README.md" << endl
        << "License: https://github.com/inonote/susjoiner/blob/master/LICENSE" << endl
        << endl;
    return 0;
  }
  
  filesystem::path baseFilePath = string(argv[1]);
  filesystem::path outputFilePath = baseFilePath.stem().string() + ".joined.sus";

  if (!exists(baseFilePath)) { // ファイル存在確認
    cout << "file not found." << endl;
    return 1;
  }

  if (filesystem::exists(outputFilePath))
    filesystem::remove(outputFilePath);
  try {
    filesystem::copy_file(baseFilePath, outputFilePath, filesystem::copy_options::overwrite_existing);
  }
  catch (...) {
    cout << "failed to copy file." << endl;
    return 1;
  }

  cout << "dest: " << outputFilePath.filename().string() << endl;

  uint32_t timelineIndex = 0;

  regex re_filepath("\\.\\/tl\\..*\\.sus");
  regex re_replace1("#[0-9a-zA-Z]*[^:] [\\s\\S]*?([\r\n])");
  regex re_replace2("#0000[28]: [0-9.]*?([\r\n])");
  regex re_replace3("#BPM[0-9A-Z][0-9A-Z]: [0-9.]*?([\r\n])");
  regex re_replace4("#TIL00: ([\\s\\S]*?)([\r\n])");
  
  ofstream outputFile(outputFilePath, ios::app);

  for(const auto &file : filesystem::directory_iterator("./")) {
    if (regex_match(file.path().string(), re_filepath)) {
      cout << "join: " << file.path().filename().string() << endl;

      timelineIndex++;
      string timelineIndexBase36 = string({ito36(timelineIndex / 36), ito36(timelineIndex % 36)});

      // ファイルを全部読み込む
      ifstream ifs(file.path().string());
      string buf((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
      
      // 要らない定義を除去したり、TILとHISPEED定義を書き換えたり
      buf = std::regex_replace(buf, re_replace1, "$1");
      buf = std::regex_replace(buf, re_replace2, "$1");
      buf = std::regex_replace(buf, re_replace3, "$1");
      buf = std::regex_replace(buf, re_replace4, string("#TIL") + timelineIndexBase36 + " $1\n#HISPEED " + timelineIndexBase36);

      // ぶち込む
      outputFile << buf << endl;
    }
  }

  outputFile.close();

  cout << "done." << endl;
  return 0;
}
