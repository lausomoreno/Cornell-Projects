import os
import cv2
import matplotlib.pyplot as plt
from roipoly import RoiPoly
import numpy as np

IMAGE_DIR = r"C:\Users\lauso\OneDrive\Desktop\Cornell\Intelligent Autonomous Systems\ECE5242Proj1-train\ECE5242Proj1-train"

train_images = []
train_filenames = []

SKIP_INDICES = {27, 6, 31, 5, 20, 17, 14}

for filename in sorted(os.listdir(IMAGE_DIR)):
    if not filename.lower().endswith(".png"):
        continue

    try:
        img_num = int(filename.split("_")[1])
    except (IndexError, ValueError):
        continue

    if img_num in SKIP_INDICES:
        continue

    img_path = os.path.join(IMAGE_DIR, filename)
    img_bgr = cv2.imread(img_path)
    if img_bgr is None:
        continue

    image = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2RGB)
    train_images.append(image)
    train_filenames.append(filename)

masks = []
mask_filenames = []

for img, fname in zip(train_images, train_filenames):
    plt.figure()
    plt.imshow(img)
    plt.title(fname)
    plt.axis("off")

    roi1 = RoiPoly(color='r')
    mask = roi1.get_mask(img[:, :, 0])

    if mask.sum() > 0:
        masks.append(mask)
        mask_filenames.append(fname)

    plt.close()
print("Saved masks:", len(masks))
print("Saved filenames:", len(mask_filenames))
np.save("cone_masks.npy", masks)
np.save("mask_filenames.npy", mask_filenames)
