import cv2
import mediapipe as mp
import math
import asyncio
import aiohttp
import collections

mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands=1, min_detection_confidence=0.7, min_tracking_confidence=0.7)
mp_drawing = mp.solutions.drawing_utils

async def send_async_request(url):
    async with aiohttp.ClientSession() as session:
        try:
            async with session.get(url) as response:
                print("Request sent")
        except aiohttp.ClientError as e:
            print(f"Request failed: {e}")

def fingers_close():
    asyncio.run(send_async_request(Your_HTTPS_Request))  # repplace it with your https request generated from Blynk Cloud
    print("Thumb and index finger are close to each other!")

def fingers_apart():
    asyncio.run(send_async_request(Your_HTTPS_Request))  # repplace it with your https request generated from Blynk Cloud
    print("Thumb and index finger are apart from each other!")

def calculate_distance(x1, y1, x2, y2):
    return math.sqrt((x2 - x1)**2 + (y2 - y1)**2)

THRESHOLD_DISTANCE = 30 
fingers_close_state = False
debounce_count = 0
DEBOUNCE_THRESHOLD = 5
distance_history = collections.deque(maxlen=5)

cap = cv2.VideoCapture(0)

while cap.isOpened():
    success, image = cap.read()
    if not success:
        break

    image = cv2.flip(image, 1)
    image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    results = hands.process(image_rgb)

    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            mp_drawing.draw_landmarks(image, hand_landmarks, mp_hands.HAND_CONNECTIONS)
            
            thumb_tip = hand_landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP]
            index_finger_tip = hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP]

            thumb_tip_coords = (int(thumb_tip.x * image.shape[1]), int(thumb_tip.y * image.shape[0]))
            index_finger_tip_coords = (int(index_finger_tip.x * image.shape[1]), int(index_finger_tip.y * image.shape[0]))

            distance = calculate_distance(thumb_tip_coords[0], thumb_tip_coords[1], index_finger_tip_coords[0], index_finger_tip_coords[1])
            distance_history.append(distance)
            average_distance = sum(distance_history) / len(distance_history)

            if average_distance < THRESHOLD_DISTANCE:
                if not fingers_close_state:
                    debounce_count += 1
                    if debounce_count > DEBOUNCE_THRESHOLD:
                        fingers_close()
                        fingers_close_state = True
                        debounce_count = 0
                else:
                    debounce_count = 0
            else:
                if fingers_close_state:
                    debounce_count += 1
                    if debounce_count > DEBOUNCE_THRESHOLD:
                        fingers_apart()
                        fingers_close_state = False
                        debounce_count = 0
                else:
                    debounce_count = 0

    cv2.imshow('Hand Tracking', image)

    if cv2.waitKey(5) & 0xFF == 27:
        break

cap.release()
cv2.destroyAllWindows()
