<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class MeasureLogs extends Model
{
    protected $fillable = [
        'timestamp',
        'anchor',
        'tag',
        'distance',
        'rssi',
    ];
}
