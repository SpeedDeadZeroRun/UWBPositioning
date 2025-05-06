<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;
use Illuminate\Database\Eloquent\Relations\BelongsTo;
use Illuminate\Database\Eloquent\Relations\HasOne;
use Illuminate\Database\Eloquent\Relations\HasMany;

class Entities extends Model
{
    protected $fillable = [
        'id',
        'type',
        'created_at',
    ];

    public function AccessZones(): HasMany
    {
        return $this->hasMany(AccessZones::class, 'entity');
    }

    public function AnchorLocation(): HasOne
    {
        return $this->hasOne(AnchorLocation::class, 'anchor');
    }

    public function ControllerHaveAnchors(): HasMany
    {
        return $this->hasMany(ControllerHaveAnchors::class, 'controller');
    }

    public function EntityTypes(): BelongsTo
    {
        return $this->belongsTo(EntityTypes::class, 'type', 'id');
    }

    public function MeasureLogs(): HasMany
    {
        return $this->hasMany(MeasureLogs::class, 'anchor', 'tag');
    }

    public function uwbIps(): HasOne
    {
        return $this->hasOne(uwbIps::class, 'id', 'uwb');
    }
}
