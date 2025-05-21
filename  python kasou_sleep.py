import pyautogui
import time
from datetime import datetime, timedelta

# 開始時刻
start_time = datetime.now()
end_time = start_time + timedelta(hours=9)

print(f"開始: {start_time.strftime('%H:%M:%S')} ～ 終了予定: {end_time.strftime('%H:%M:%S')}")
print("Slackオンライン維持を開始します...")

# 無限ループ
while datetime.now() < end_time:
    # 軽くマウスを動かす（オンライン状態維持のため）
    x, y = pyautogui.position()
    pyautogui.moveTo(x + 1, y + 1, duration=0.1)
    pyautogui.moveTo(x, y, duration=0.1)
    print(f"[{datetime.now().strftime('%H:%M:%S')}] アクション実行")

    # 4分（240秒）待機
    time.sleep(240)

print("9時間の維持が完了しました。Slackオンライン維持を終了します。")