#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>  // _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#define RANK_FILE_PATH "assets/score_rank.txt"
#define RANK_DIR "assets"
#define MAX_RECORDS 100

void ensureDirectoryExists() {
#ifdef _WIN32
    _mkdir(RANK_DIR);
#else
    mkdir(RANK_DIR, 0777);
#endif
}

int loadAllScores(int scores[], int maxScores) {
    FILE* file = fopen(RANK_FILE_PATH, "r");
    if (!file) return 0;

    int count = 0;
    while (!feof(file) && count < maxScores) {
        char line[256];
        if (fgets(line, sizeof(line), file)) {
            int sec = 0;
            if (sscanf(line, "%*[^]] 생존 시간: %d초", &sec) == 1) {
                scores[count++] = sec;
            }
        }
    }
    fclose(file);
    return count;
}

int getRank(int scores[], int total, int newScore) {
    int rank = 1;
    for (int i = 0; i < total; i++) {
        if (scores[i] < newScore) {
            rank++;  // 기존 기록보다 더 오래 생존했다면 내 순위는 더 앞임
        }
    }
    return total - rank + 1;  // 높은 생존 시간이 1위가 되도록 보정
}


void saveScoreWithTime(int secondsSurvived) {
    ensureDirectoryExists();

    // 기존 점수 불러오기
    int scores[MAX_RECORDS];
    int totalRecords = loadAllScores(scores, MAX_RECORDS);

    int rank = getRank(scores, totalRecords, secondsSurvived);

    // 현재 시간
    time_t now = time(NULL);
    struct tm* t = localtime(&now);

    FILE* file = fopen(RANK_FILE_PATH, "a");
    if (file == NULL) {
        perror("랭킹 파일 열기 실패");
        return;
    }

    fprintf(file, "(%d위 [%04d-%02d-%02d %02d:%02d:%02d] 생존 시간: %d초)\n",
        rank,
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
        t->tm_hour, t->tm_min, t->tm_sec,
        secondsSurvived);

    fclose(file);
}
