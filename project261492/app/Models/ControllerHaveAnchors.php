<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class ControllerHaveAnchors extends Model
{
    protected $fillable = [
        'controller',
        'have_anchor',
        'on_port',
    ];
}
