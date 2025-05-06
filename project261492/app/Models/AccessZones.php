<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Relations\BelongsTo;
use Illuminate\Database\Eloquent\Model;

class AccessZones extends Model
{
    public function Entities(): BelongsTo
    {
        return $this->belongsTo(Zones::class, 'access_zone', 'id');
    }
}
