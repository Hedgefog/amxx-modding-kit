# ♟️ Custom Entities API

The **Custom Entities API** provides a flexible OOP-style framework for managing and creating custom entities in AMX Mod X. This API allows developers to register entity classes, implement their behavior through methods, and interact with custom entities programmatically. Entities integrate seamlessly with game maps — just place them using the registered classname.

## 🚀 Features

- OOP-style entity class system with inheritance
- Preset base classes for common entity types (items, props, triggers, monsters)
- Custom members for storing entity-specific data
- Custom methods for extending entity behavior
- Key-value bindings for map editor integration
- Native method hooks for intercepting entity behavior
- Full lifecycle control (create, spawn, destroy, respawn)

---

## ⚙️ Getting Started

### Registering a New Entity Class

To create a custom entity, register a new entity class using `CE_RegisterClass` in `plugin_precache`. This allows you to place entities directly on the map using the registered classname.

Let's create a simple key item:

```pawn
#include <api_custom_entities>

public plugin_precache() {
  CE_RegisterClass("item_key", CE_Class_BaseItem);
}
```

The `CE_Class_BaseItem` preset provides built-in item logic including pickup handling.

**Available Preset Classes:**
- `CE_Class_Base` — Basic entity
- `CE_Class_BaseItem` — Pickable item
- `CE_Class_BaseProp` — Static prop
- `CE_Class_BaseTrigger` — Trigger zone
- `CE_Class_BaseMonster` — NPC entity
- `CE_Class_BaseBsp` — BSP brush entity

### Extending an Existing Class

You can inherit from an existing entity class to extend its behavior:

```pawn
public plugin_precache() {
  CE_RegisterClass("item_gold_key", "item_key");
}
```

This creates `item_gold_key` that inherits all properties and methods from `item_key`.

---

## 🛠 Implementing Entity Methods

### The Create Method

The `Create` method initializes entity members when an instance is allocated. Use it to set default values.

```pawn
new const g_szModel[] = "models/w_security.mdl";

public plugin_precache() {
  precache_model(g_szModel);

  CE_RegisterClass("item_key", CE_Class_BaseItem);
  CE_ImplementClassMethod("item_key", CE_Method_Create, "@KeyItem_Create");
}

@KeyItem_Create(const this) {
  CE_CallBaseMethod();

  CE_SetMemberString(this, CE_Member_szModel, g_szModel);
  CE_SetMemberVec(this, CE_Member_vecMins, Float:{-8.0, -8.0, 0.0});
  CE_SetMemberVec(this, CE_Member_vecMaxs, Float:{8.0, 8.0, 8.0});
}
```

> [!CAUTION]
> The `Create` method is called during entity allocation. Do not modify entity variables (`pev`) or invoke engine functions here — use it only for initializing custom entity members!

> [!CAUTION]
> Always call `CE_CallBaseMethod()` with all method arguments to ensure the parent class executes its logic properly.

**Common Members:**
- `CE_Member_szModel` — Entity model path
- `CE_Member_vecMins` / `CE_Member_vecMaxs` — Bounding box
- `CE_Member_flRespawnTime` — Respawn delay
- `CE_Member_flLifeTime` — Entity lifetime

### Writing Entity Logic

Let's implement `CanPickup` and `Pickup` methods to add behavior to our key item:

```pawn
new bool:g_rgbPlayerHasKey[MAX_PLAYERS + 1];

public plugin_precache() {
  CE_RegisterClass("item_key", CE_Class_BaseItem);
  
  CE_ImplementClassMethod("item_key", CE_Method_Create, "@KeyItem_Create");
  CE_ImplementClassMethod("item_key", CE_Method_CanPickup, "@KeyItem_CanPickup");
  CE_ImplementClassMethod("item_key", CE_Method_Pickup, "@KeyItem_Pickup");
}

@KeyItem_Create(const this) { /* ... */ }

@KeyItem_CanPickup(const this, const pPlayer) {
  // Base implementation checks if item is on ground
  if (!CE_CallBaseMethod(pPlayer)) return false;
  
  // Prevent pickup if player already has a key
  if (g_rgbPlayerHasKey[pPlayer]) return false;
  
  return true;
}

@KeyItem_Pickup(const this, const pPlayer) {
  CE_CallBaseMethod(pPlayer);
  
  g_rgbPlayerHasKey[pPlayer] = true;
  client_print(pPlayer, print_center, "You have found a key!");
}
```

---

## 🧩 Custom Members

Custom members allow you to store entity-specific data. Let's add different key types:

```pawn
#include <amxmodx>
#include <fakemeta>

#include <api_custom_entities>

#define ENTITY_NAME "item_key"

#define m_iType "iType"

enum KeyType {
  KeyType_Red = 0,
  KeyType_Yellow,
  KeyType_Green,
  KeyType_Blue
};

new const KEY_NAMES[KeyType][] = { "red", "yellow", "green", "blue" };

new const Float:KEY_COLORS_F[KeyType][3] = {
  {255.0, 0.0, 0.0},
  {255.0, 255.0, 0.0},
  {0.0, 255.0, 0.0},
  {0.0, 0.0, 255.0}
};

new const g_szModel[] = "models/w_security.mdl";

new bool:g_rgbPlayerHasKey[MAX_PLAYERS + 1][KeyType];

public plugin_precache() {
  precache_model(g_szModel);

  CE_RegisterClass(ENTITY_NAME, CE_Class_BaseItem);
  
  CE_ImplementClassMethod(ENTITY_NAME, CE_Method_Create, "@KeyItem_Create");
  CE_ImplementClassMethod(ENTITY_NAME, CE_Method_Spawn, "@KeyItem_Spawn");
  CE_ImplementClassMethod(ENTITY_NAME, CE_Method_CanPickup, "@KeyItem_CanPickup");
  CE_ImplementClassMethod(ENTITY_NAME, CE_Method_Pickup, "@KeyItem_Pickup");

  // Bind map key-value "type" to member "m_iType"
  CE_RegisterClassKeyMemberBinding(ENTITY_NAME, "type", m_iType, CEMemberType_Cell);
}

@KeyItem_Create(const this) {
  CE_CallBaseMethod();

  CE_SetMemberString(this, CE_Member_szModel, g_szModel);
  CE_SetMemberVec(this, CE_Member_vecMins, Float:{-8.0, -8.0, 0.0}); 
  CE_SetMemberVec(this, CE_Member_vecMaxs, Float:{8.0, 8.0, 8.0});

  CE_SetMember(this, m_iType, KeyType_Red); // Default type
}

@KeyItem_Spawn(const this) {
  CE_CallBaseMethod();

  new KeyType:iType = CE_GetMember(this, m_iType);

  // Apply glow effect based on key type
  set_pev(this, pev_renderfx, kRenderFxGlowShell);
  set_pev(this, pev_renderamt, 1.0);
  set_pev(this, pev_rendercolor, KEY_COLORS_F[iType]);
}

@KeyItem_CanPickup(const this, const pPlayer) {
  if (!CE_CallBaseMethod(pPlayer)) return false;

  new KeyType:iType = CE_GetMember(this, m_iType);

  if (g_rgbPlayerHasKey[pPlayer][iType]) return false;

  return true;
}

@KeyItem_Pickup(const this, const pPlayer) {
  CE_CallBaseMethod(pPlayer);

  new KeyType:iType = CE_GetMember(this, m_iType);

  g_rgbPlayerHasKey[pPlayer][iType] = true;
  client_print(pPlayer, print_center, "You have found a %s key!", KEY_NAMES[iType]);
}
```

Using `CE_RegisterClassKeyMemberBinding`, you can set the key type directly in the map editor via the `type` key-value.

---

## 📦 Custom Methods

Custom methods extend entity behavior beyond built-in methods. Let's add `SetType` and `UpdateColor` methods:

```pawn
#define SetType "SetType"
#define UpdateColor "UpdateColor"

public plugin_precache() {
  CE_RegisterClass(ENTITY_NAME, CE_Class_BaseItem);

  // Register custom methods
  CE_RegisterClassMethod(ENTITY_NAME, SetType, "@KeyItem_SetType", CE_Type_Cell);
  CE_RegisterClassMethod(ENTITY_NAME, UpdateColor, "@KeyItem_UpdateColor");

  CE_ImplementClassMethod(ENTITY_NAME, CE_Method_Create, "@KeyItem_Create");
  CE_ImplementClassMethod(ENTITY_NAME, CE_Method_Spawn, "@KeyItem_Spawn");
  /* ... */
}

@KeyItem_Spawn(const this) {
  CE_CallBaseMethod();

  // Use custom method to set color
  CE_CallMethod(this, UpdateColor);
}

@KeyItem_SetType(const this, const KeyType:iType) {
  CE_SetMember(this, m_iType, iType);

  // Update color when type changes
  CE_CallMethod(this, UpdateColor);
}

@KeyItem_UpdateColor(const this) {
  new KeyType:iType = CE_GetMember(this, m_iType);

  set_pev(this, pev_renderfx, kRenderFxGlowShell);
  set_pev(this, pev_renderamt, 1.0);
  set_pev(this, pev_rendercolor, KEY_COLORS_F[iType]);
}
```

Now you can change the key type at runtime using `CE_CallMethod(pKey, "SetType", KeyType_Blue)`.

---

## 🔧 Hooks

Hook into native methods to intercept entity behavior:

```pawn
public plugin_precache() {
  CE_RegisterClass(ENTITY_NAME, CE_Class_BaseItem);
  CE_RegisterClassNativeMethodHook(ENTITY_NAME, CE_Method_Spawn, "CEHook_KeyItem_Spawn");
}

public CEHook_KeyItem_Spawn(const pEntity) {
  // Execute custom logic when any key item spawns
  log_amx("Key item spawned: %d", pEntity);
}
```

Use the `bPost` parameter to hook after the method executes:

```pawn
CE_RegisterClassNativeMethodHook(ENTITY_NAME, CE_Method_Spawn, "CEHook_KeyItem_Spawn_Post", true);
```

---

## 🕵️‍♂️ Testing and Debugging

### Console Commands

**Spawn an entity:**
```bash
ce_spawn "item_key" "iType" 3
```

**Call a method on an entity:**
```bash
ce_call_method <entity_index> "item_key" "SetType" 3
```

**List all custom entities:**
```bash
ce_list
```

### Spawning via Code

```pawn
new Float:vecOrigin[3] = {0.0, 0.0, 0.0};
new pKey = CE_Create("item_key", vecOrigin);

if (pKey != FM_NULLENT) {
  CE_SetMember(pKey, m_iType, KeyType_Blue);
  dllfunc(DLLFunc_Spawn, pKey);
}
```

Or use custom methods:

```pawn
CE_CallMethod(pKey, "SetType", KeyType_Blue);
```

---

## 📖 API Reference

See [`api_custom_entities.inc`](include/api_custom_entities.inc) and [`api_custom_entities_const.inc`](include/api_custom_entities_const.inc) for all available natives and constants.
