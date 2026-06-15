![QuickAxisAlign](https://img.shields.io/badge/QuickAxisAlign-v1.0-blue?style=for-the-badge)

# Quick Axis Align

Precision transform alignment plugin for Unreal Engine.

Quickly align multiple actors by Location, Rotation, or Scale with per-channel control. Select your sources, pick a target, and apply the exact transform channels you need — all from a dedicated editor panel or through an interactive visual picker in the viewport.

⭐⭐ **If you found it useful, please consider leaving a review to help us keep updating the product.** ⭐⭐

---

## Compatible Engine Version

| Unreal Engine | Plugin Version | Support |
| ------------- | -------------- | ------- |
| 5.7           | 1.0            | ✅      |

---

## What It Does

- Aligns selected source actors to the last selected target actor.
- Lets you choose individual X, Y, and Z channels for Location, Rotation, and Scale.
- Supports multiple sources in one apply action.
- Includes Undo/Redo support through Unreal transactions.
- Includes Visual Align mode for point-based alignment in the viewport.

---

## How To Use: Transform Channel Align

Use this mode when you want to copy selected transform values from a target actor to one or more source actors.

![Image](https://github.com/user-attachments/assets/a04abba8-08fa-408d-9736-08619df48864)

1. Select the actor or actors you want to move. These are the **Sources**.
2. Select the actor you want to match last. This is the **Target**.
3. Open **Tools > Quick Axis Align > Quick Axis Align**.

![Image](https://github.com/user-attachments/assets/4bbd59f9-628f-4ff4-8b93-55eb4335d16e)

4. In the panel, enable the channels you want to copy:
    - **Location X/Y/Z** copies target position axes.
    - **Rotation X/Y/Z** copies target rotation axes.
    - **Scale X/Y/Z** copies target scale axes.
5. Click **Apply**.

![Image](https://github.com/user-attachments/assets/44b46c6f-61a5-45d2-813d-4151132deac6)

---

## How To Use: Visual Align

Use Visual Align when you want to snap a point on one actor to a point on another actor directly in the viewport.

![Image](https://github.com/user-attachments/assets/0bd6150a-9a3a-4e23-874b-92d791cb3a26)

1. Open **Tools > Quick Axis Align > Quick Axis Align**.
2. In the **Visual Align** section, choose a mode:
    - **Move objects only** moves the source point onto the target point.
    - **Move and rotate objects** moves the source point and rotates the source surface normal against the target surface normal.
3. Click **Visual Align**.
4. Left-click a point on the source actor.
5. Left-click a point on the target actor.
6. Press **Enter** or click **Apply** to confirm.
7. Press **Esc** to cancel at any time.

You can also start Visual Align directly from **Tools > Quick Axis Align > Visual Align**, or by pressing **ALT + X**.

![Image](https://github.com/user-attachments/assets/bfe388c0-e60f-47da-83bd-7a828956f4c9)

---

## Tips

- Double-click a transform row label, such as **Location**, **Rotation**, or **Scale**, to toggle all three axes in that row.
- Use normal transform alignment for batch changes across many actors.
- Use Visual Align for contact points, surface matching, pivots that are not convenient, or irregular meshes.
- You can also start Visual Align using the **ALT + X** keyboard shortcut.
- If **Apply** is disabled, make sure at least two actors are selected for transform channel alignment.

---

## About

Quick Axis Align helps artists and level designers speed up actor alignment in the editor. Perfect for kitbashing, prototying, or any workflow that requires precise multi-actor transforms without manual adjustments.

**Resources:**

- 🎮 [Unreal Marketplace](https://www.fab.com/listings)
- 🐛 [Report issues](https://github.com/Glowlinestd/QuickAxisAlign/issues)
