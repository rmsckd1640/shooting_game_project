#ifndef SCORE_H
#define SCORE_H

/**
 * 생존 시간 저장 함수
 *
 * - 실행 시 assets/score_rank.txt에 자동으로 저장됨
 * - 기존 기록들과 비교하여 순위를 계산한 후 다음 형식으로 저장:
 *     (2위 [2025-06-08 02:08:27] 생존 시간: 50초)
 *
 * - 자동으로 assets 폴더가 없으면 생성함
 *
 * @param secondsSurvived 생존 시간 (초 단위)
 */
void saveScoreWithTime(int secondsSurvived);

#endif
