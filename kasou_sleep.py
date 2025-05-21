import pyautogui
import time
from datetime import datetime, timedelta

# ✅ 終了時間：9時間後
start_time = datetime.now()
end_time = start_time + timedelta(hours=9)

print(f"開始: {start_time.strftime('%H:%M:%S')} ～ 終了予定: {end_time.strftime('%H:%M:%S')}")
print("Slackオンライン維持を開始します...")

# ✅ マウスを定期的に動かしてSlackオンライン維持
while datetime.now() < end_time:
    x, y = pyautogui.position()
    pyautogui.moveTo(x + 1, y, duration=0.1)     # 少し横に動かす
    pyautogui.moveTo(x, y, duration=0.1)         # 元に戻す
    pyautogui.click()                            # 軽くクリック
    pyautogui.press('shift')                     # キーボード操作
    print(f"[{datetime.now().strftime('%H:%M:%S')}] アクション実行")
    time.sleep(60)                               # 1分ごとに実行

print("🔚 9時間のマウス操作が完了しました")