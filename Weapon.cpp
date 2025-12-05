#include "Weapon.hpp"
#include "Bullet.hpp"

Weapon::Weapon(float fr, int dmg) : fireRate(fr), cooldown(0.f), damage(dmg) {}

void Weapon::update(float dt) {
    if (cooldown > 0.f) cooldown -= dt;
}

bool Weapon::canFire() const {
    return cooldown <= 0.f;
}

std::unique_ptr<Bullet> Weapon::fire(const sf::Vector2f& from, const sf::Vector2f& dir) {
    if (!canFire()) return nullptr;
    cooldown = 1.f / fireRate;
    return std::make_unique<Bullet>(from, dir, 700.f, 2.0f, damage);
}

void Weapon::setFireRate(float f) { fireRate = f; }
void Weapon::setDamage(int d) { damage = d; }
float Weapon::getFireRate() const { return fireRate; }
int Weapon::getDamage() const { return damage; }